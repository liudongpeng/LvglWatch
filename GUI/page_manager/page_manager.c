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
#define CHECK_PAGE_ID(id) ((id) > 0 && (id) < (pm->max_page_num))


/**
 * @brief 创建一个界面
 * @param[out]	page
 * @param[in]	page_setup
 * @param[in]	page_loop
 * @param[in]	page_exit
 * @param[in]	page_event_handle
 * @return
 */
int page_create(page_t *page, page_cb_t page_setup, page_cb_t page_loop, page_cb_t page_exit,
                page_event_cb_t page_event_handle)
{
	/* 界面id, 编号从1开始递增, 因为page_new和page_old初始值都是0, 界面从0开始编号会影响界面切换 */
	static uint8_t s_page_id = 1;

	if (page == NULL)
		return -1;

	/* 界面编号自增1 */
	page->page_id = s_page_id++;

	page->page_addr = page;

	page->page_setup = page_setup;
	page->page_loop = page_loop;
	page->page_exit = page_exit;
	page->page_event_handle = page_event_handle;

	return 0;
}

/**
 * @brief 通过id号拿到page句柄
 * @param[in]	pm
 * @param[in]	id
 * @return
 */
page_t *get_page_by_id(page_manager_t *pm, uint8_t id)
{
	if (pm == NULL)
		return NULL;

//	if (!CHECK_PAGE_ID(id))
//		return NULL;

	return pm->page_list[id].page_addr;
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

	pm->max_page_num = max_page_num;
	pm->page_stack_size = page_stack_size;
	pm->page_stack_top = -1;

	if ((pm->page_list = pvPortMalloc(sizeof(page_t) * max_page_num)) == NULL)
		return -2;
	for (int i = 0; i < max_page_num; i++)
		page_clear(pm, i);

	if ((pm->page_stack = pvPortMalloc(sizeof(uint8_t) * page_stack_size)) == NULL)
		return -3;
	page_stack_clear(pm);

	pm->old_page = 0;
	pm->new_page = 1;

	return 0;
}

/**
 * @brief 注册界面到界面管理器, 统一管理
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_register(page_manager_t *pm, uint8_t id, page_cb_t page_setup, page_cb_t page_loop, page_cb_t page_exit,
                  page_event_cb_t page_event_handle)
{
	if (pm == NULL)
		return -1;

	if (!CHECK_PAGE_ID(id))
		return -2;

	pm->page_list[id].page_setup = page_setup;
	pm->page_list[id].page_loop = page_loop;
	pm->page_list[id].page_exit = page_exit;
	pm->page_list[id].page_event_handle = page_event_handle;

	printf("in page_register(), id = %d, ", id);
	printf("page exit: %p\n", pm->page_list[id].page_exit);

	return 0;
}

/**
 * @brief 在界面管理器中清除此界面
 * @param[in]	pm
 * @param[in]	page
 */
void page_clear(page_manager_t *pm, uint8_t id)
{
	if (pm == NULL)
		return;

	if (!CHECK_PAGE_ID(id))
		return;

	pm->page_list[id].page_setup = NULL;
	pm->page_list[id].page_loop = NULL;
	pm->page_list[id].page_exit = NULL;
	pm->page_list[id].page_event_handle = NULL;
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

	pm->page_stack_top = -1;
}

/**
 * @brief 此界面压栈
 * @param[in]	pm
 * @param[in]	id
 * @return
 */
int page_push_by_id(page_manager_t *pm, uint8_t id)
{
	if (pm == NULL)
		return -1;

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
	return page_change(pm, id);
}

/**
 * @brief 把此界面压栈
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_push(page_manager_t *pm, uint8_t id)
{
	if (pm == NULL)
		return -1;

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
	pm->page_stack[++pm->page_stack_top] = id;

	/* 跳转到此界面 */
	return page_change(pm, id);
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

	/* 界面是否繁忙 */
	if (pm->is_busy)
		return -2;

	/* 栈空, 不做操作 */
	if (pm->page_stack_top <= -1)
		return -2;

	/* 清空此界面 */
	pm->page_stack[pm->page_stack_top] = 0;

	/* 切换到当前栈顶的界面 */
	return page_change(pm, pm->page_stack[--pm->page_stack_top]);
}

/**
 * @brief 切换到指定界面
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_change(page_manager_t *pm, uint8_t id)
{
	if (pm == NULL)
		return -1;

	if (!CHECK_PAGE_ID(id))
		return -2;

	/* 检查界面是否繁忙 */
	if (!pm->is_busy)
	{
		/* 记录新界面id, 在page_run()中进行界面切换 */
		pm->next_page = pm->new_page = id;
		pm->is_busy = 1;

		return 0;
	}

	return -3;
}

/**
 * @brief 将事件传递到此界面
 * @param[in]	pm
 * @param[in]	obj
 * @param[in]	event
 */
void page_event_transmit(page_manager_t *pm, void *obj, int event)
{
	if (pm == NULL)
		return;

	/* 把按键事件传递到界面 */
	if (pm->page_list[pm->cur_page].page_event_handle != NULL)
		pm->page_list[pm->cur_page].page_event_handle(obj, event);
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

		printf("page change, old = %d, new = %d\n", pm->old_page, pm->new_page);
		printf("pm->page_list[pm->old_page].page_exit = %p\n", pm->page_list[pm->old_page].page_exit);

		/* 触发旧界面退出事件 */
		if (CHECK_PAGE_ID(pm->old_page) && pm->page_list[pm->old_page].page_exit != NULL)
		{
			pm->page_list[pm->old_page].page_exit();
		}

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
