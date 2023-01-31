//
// Created by QT on 2023/1/28.
//

#include "page_manager.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief 界面id合法性检查
 */
#define CHECK_PAGE_ID(id) ((id) < (pm->max_page_num))


/**
 * @brief 创建一个界面
 * @param[out]	page
 * @param[in]	page_setup
 * @param[in]	page_loop
 * @param[in]	page_exit
 * @param[in]	page_event_handle
 * @return
 */
int page_create(page_t *page, callback_func_t page_setup, callback_func_t page_loop, callback_func_t page_exit,
                event_func_t page_event_handle)
{
	/* 界面id, 编号从1开始递增, 因为page_new和page_old初始值都是0, 界面从0开始编号会影响界面切换 */
	static uint8_t s_page_id = 1;

	if (page == NULL)
		return -1;

	/* 界面编号自增1 */
	page->page_id = s_page_id++;

	page->page_setup = page_setup;
	page->page_loop = page_loop;
	page->page_exit = page_exit;
	page->page_event_handle = page_event_handle;

	return 0;
}

/**
 * @brief 创建界面管理器
 * @param[out]	pm
 * @param[in]	max_page_num
 * @param[in]	page_stack_size
 * @return
 */
int page_manager_create(page_manager_t *pm, uint8_t max_page_num, uint8_t page_stack_size)
{
	if (pm == NULL)
		return -1;

	memset(pm, 0, sizeof(page_manager_t));

	pm->max_page_num = max_page_num;
	pm->page_stack_size = page_stack_size;

	if ((pm->page_list = pvPortMalloc(sizeof(page_t) * max_page_num)) == NULL)
		return -2;

	if ((pm->page_stack = pvPortMalloc(sizeof(uint8_t) * page_stack_size)) == NULL)
		return -3;

	pm->page_stack_top = -1;

	return 0;
}

/**
 * @brief 注册界面到界面管理器, 统一管理
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_register(page_manager_t *pm, page_t *page)
{
	uint8_t id;

	if (pm == NULL || page == NULL)
		return -1;

	id = page->page_id;
	if (!CHECK_PAGE_ID(id))
		return -2;

	pm->page_list[id].page_setup = page->page_setup;
	pm->page_list[id].page_loop = page->page_loop;
	pm->page_list[id].page_exit = page->page_exit;
	pm->page_list[id].page_event_handle = page->page_event_handle;

	return 0;
}

/**
 * @brief 在界面管理器中清除此界面
 * @param[in]	pm
 * @param[in]	page
 */
void page_clear(page_manager_t *pm, page_t *page)
{
	uint8_t id;

	if (pm == NULL || page == NULL)
		return;

	id = page->page_id;
	if (!CHECK_PAGE_ID(id))
		return;

	/* 清空此界面所有的回调, 暂时保留此界面的id */
	memset(&pm->page_list[id], 0, sizeof(page_t));
	pm->page_list[id].page_id = id;
}

/**
 * @brief 清空界面管理器的栈结构
 * @param pm
 */
void page_stack_clear(page_manager_t *pm)
{
	if (pm == NULL)
		return;

	if (pm->is_busy)
		return;

	for (int i = 0; i < pm->page_stack_size; i++)
	{
		pm->page_stack[i] = 0;
	}

	pm->page_stack_top = 0;
}

/**
 * @brief 把此界面压栈
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_push(page_manager_t *pm, page_t *page)
{
	uint8_t id;

	if (pm == NULL || page == NULL)
		return -1;

	id = page->page_id;
	if (!CHECK_PAGE_ID(id))
		return -2;

	/* 界面是否繁忙 */
	if (pm->is_busy)
		return -3;

	/* 防止栈溢出 */
	if (pm->page_stack_top >= pm->page_stack_size)
		return -4;

	/* 防止重复压栈 */
	if (pm->page_stack[pm->page_stack_top] == id)
		return -5;

	/* 压栈 */
	pm->page_stack[(pm->page_stack_top)++] = id;

	/* 跳转到此界面 */
	return page_change(pm, page);
}

/**
 * @brief 此界面出栈
 * @param[in]	pm
 * @return
 */
int page_pop(page_manager_t *pm)
{
	uint8_t id;

	if (pm == NULL)
		return -1;

	id = pm->page_stack[pm->page_stack_top - 1];
	if (!CHECK_PAGE_ID(id))
		return -2;

	/* 界面是否繁忙 */
	if (pm->is_busy)
		return -3;

	/* 防止栈溢出 */
	if (pm->page_stack_top <= -1)
		return -4;

	/* 清空此界面 */
	page_clear(pm, &pm->page_list[id]);

	/* 弹栈 */
	(pm->page_stack_top)--;

	/* 栈空, 不做操作 */
	if (pm->page_stack_top == 0)
		return -5;

	/* 切换到当前栈顶的界面 */
	return page_change(pm, &pm->page_list[pm->page_stack[pm->page_stack_top - 1]]);
}

/**
 * @brief 切换到指定界面
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_change(page_manager_t *pm, page_t *page)
{
	uint8_t id;

	if (pm == NULL || page == NULL)
		return -1;

	/* 检查界面是否繁忙 */
	if (pm->is_busy)
		return -2;

	id = page->page_id;
	if (!CHECK_PAGE_ID(id))
		return -3;

	/* 记录新界面id, 在page_run()中进行界面切换 */
	pm->next_page = pm->new_page = id;

	pm->is_busy = 1;

	return 0;
}

/**
 * @brief 将事件传递到此界面
 * @param[in]	pm
 * @param[in]	obj
 * @param[in]	event
 * @return
 */
int page_event_transmit(page_manager_t *pm, void *obj, int event)
{
	if (pm == NULL || obj == NULL)
		return -1;

	if (pm->page_list[pm->cur_page].page_event_handle == NULL)
		return -2;

	/* 调用此界面的事件处理接口 */
	pm->page_list[pm->cur_page].page_event_handle(obj, event);

	return 0;
}

/**
 * @brief 界面调度
 * @param[in]	pm
 */
void page_run(page_manager_t *pm)
{
	if (pm == NULL)
		return;

	/* 界面切换事件 */
	if (pm->new_page != pm->old_page)
	{
		/* 标记为繁忙 */
		pm->is_busy = 1;

		/* 触发旧界面退出事件 */
		if (CHECK_PAGE_ID(pm->old_page) && pm->page_list[pm->old_page].page_exit != NULL)
			pm->page_list[pm->old_page].page_exit();

		/* 标记旧界面 */
		pm->last_page = pm->old_page;

		/* 记录当前界面 */
		pm->cur_page = pm->new_page;

		/* 触发新界面初始化事件 */
		if (CHECK_PAGE_ID(pm->new_page) && pm->page_list[pm->new_page].page_setup != NULL)
		{
			pm->page_list[pm->new_page].page_setup();
		}

		/* 新界面初始化完成, 标记为旧界面 */
		pm->old_page = pm->new_page;
	}
	else
	{
		/* 标记界面不忙碌, 处于循环状态 */
		pm->is_busy = 0;

		/* 界面循环事件 */
		if (CHECK_PAGE_ID(pm->cur_page) && pm->page_list[pm->cur_page].page_loop != NULL)
		{
			pm->page_list[pm->cur_page].page_loop();
		}
	}
}

