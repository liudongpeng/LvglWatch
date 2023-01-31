//
// Created by QT on 2023/1/28.
//

#include "page_manager.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief ����id�Ϸ��Լ��
 */
#define CHECK_PAGE_ID(id) ((id) < (pm->max_page_num))


/**
 * @brief ����һ������
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
	/* ����id, ��Ŵ�1��ʼ����, ��Ϊpage_new��page_old��ʼֵ����0, �����0��ʼ��Ż�Ӱ������л� */
	static uint8_t s_page_id = 1;

	if (page == NULL)
		return -1;

	/* ����������1 */
	page->page_id = s_page_id++;

	page->page_setup = page_setup;
	page->page_loop = page_loop;
	page->page_exit = page_exit;
	page->page_event_handle = page_event_handle;

	return 0;
}

/**
 * @brief �������������
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
 * @brief ע����浽���������, ͳһ����
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
 * @brief �ڽ��������������˽���
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

	/* ��մ˽������еĻص�, ��ʱ�����˽����id */
	memset(&pm->page_list[id], 0, sizeof(page_t));
	pm->page_list[id].page_id = id;
}

/**
 * @brief ��ս����������ջ�ṹ
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
 * @brief �Ѵ˽���ѹջ
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

	/* �����Ƿ�æ */
	if (pm->is_busy)
		return -3;

	/* ��ֹջ��� */
	if (pm->page_stack_top >= pm->page_stack_size)
		return -4;

	/* ��ֹ�ظ�ѹջ */
	if (pm->page_stack[pm->page_stack_top] == id)
		return -5;

	/* ѹջ */
	pm->page_stack[(pm->page_stack_top)++] = id;

	/* ��ת���˽��� */
	return page_change(pm, page);
}

/**
 * @brief �˽����ջ
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

	/* �����Ƿ�æ */
	if (pm->is_busy)
		return -3;

	/* ��ֹջ��� */
	if (pm->page_stack_top <= -1)
		return -4;

	/* ��մ˽��� */
	page_clear(pm, &pm->page_list[id]);

	/* ��ջ */
	(pm->page_stack_top)--;

	/* ջ��, �������� */
	if (pm->page_stack_top == 0)
		return -5;

	/* �л�����ǰջ���Ľ��� */
	return page_change(pm, &pm->page_list[pm->page_stack[pm->page_stack_top - 1]]);
}

/**
 * @brief �л���ָ������
 * @param[in]	pm
 * @param[in]	page
 * @return
 */
int page_change(page_manager_t *pm, page_t *page)
{
	uint8_t id;

	if (pm == NULL || page == NULL)
		return -1;

	/* �������Ƿ�æ */
	if (pm->is_busy)
		return -2;

	id = page->page_id;
	if (!CHECK_PAGE_ID(id))
		return -3;

	/* ��¼�½���id, ��page_run()�н��н����л� */
	pm->next_page = pm->new_page = id;

	pm->is_busy = 1;

	return 0;
}

/**
 * @brief ���¼����ݵ��˽���
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

	/* ���ô˽�����¼�����ӿ� */
	pm->page_list[pm->cur_page].page_event_handle(obj, event);

	return 0;
}

/**
 * @brief �������
 * @param[in]	pm
 */
void page_run(page_manager_t *pm)
{
	if (pm == NULL)
		return;

	/* �����л��¼� */
	if (pm->new_page != pm->old_page)
	{
		/* ���Ϊ��æ */
		pm->is_busy = 1;

		/* �����ɽ����˳��¼� */
		if (CHECK_PAGE_ID(pm->old_page) && pm->page_list[pm->old_page].page_exit != NULL)
			pm->page_list[pm->old_page].page_exit();

		/* ��Ǿɽ��� */
		pm->last_page = pm->old_page;

		/* ��¼��ǰ���� */
		pm->cur_page = pm->new_page;

		/* �����½����ʼ���¼� */
		if (CHECK_PAGE_ID(pm->new_page) && pm->page_list[pm->new_page].page_setup != NULL)
		{
			pm->page_list[pm->new_page].page_setup();
		}

		/* �½����ʼ�����, ���Ϊ�ɽ��� */
		pm->old_page = pm->new_page;
	}
	else
	{
		/* ��ǽ��治æµ, ����ѭ��״̬ */
		pm->is_busy = 0;

		/* ����ѭ���¼� */
		if (CHECK_PAGE_ID(pm->cur_page) && pm->page_list[pm->cur_page].page_loop != NULL)
		{
			pm->page_list[pm->cur_page].page_loop();
		}
	}
}

