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
#define CHECK_PAGE_ID(id) ((id) > 0 && (id) < (pm->max_page_num))


/**
 * @brief ����һ������
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
	/* ����id, ��Ŵ�1��ʼ����, ��Ϊpage_new��page_old��ʼֵ����0, �����0��ʼ��Ż�Ӱ������л� */
	static uint8_t s_page_id = 1;

	if (page == NULL)
		return -1;

	/* ����������1 */
	page->page_id = s_page_id++;

	page->page_addr = page;

	page->page_setup = page_setup;
	page->page_loop = page_loop;
	page->page_exit = page_exit;
	page->page_event_handle = page_event_handle;

	return 0;
}

/**
 * @brief ͨ��id���õ�page���
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
 * @brief ע����浽���������, ͳһ����
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
 * @brief �ڽ��������������˽���
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

	pm->page_stack_top = -1;
}

/**
 * @brief �˽���ѹջ
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
	return page_change(pm, id);
}

/**
 * @brief �Ѵ˽���ѹջ
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
	pm->page_stack[++pm->page_stack_top] = id;

	/* ��ת���˽��� */
	return page_change(pm, id);
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

	/* �����Ƿ�æ */
	if (pm->is_busy)
		return -2;

	/* ջ��, �������� */
	if (pm->page_stack_top <= -1)
		return -2;

	/* ��մ˽��� */
	pm->page_stack[pm->page_stack_top] = 0;

	/* �л�����ǰջ���Ľ��� */
	return page_change(pm, pm->page_stack[--pm->page_stack_top]);
}

/**
 * @brief �л���ָ������
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

	/* �������Ƿ�æ */
	if (!pm->is_busy)
	{
		/* ��¼�½���id, ��page_run()�н��н����л� */
		pm->next_page = pm->new_page = id;
		pm->is_busy = 1;

		return 0;
	}

	return -3;
}

/**
 * @brief ���¼����ݵ��˽���
 * @param[in]	pm
 * @param[in]	obj
 * @param[in]	event
 */
void page_event_transmit(page_manager_t *pm, void *obj, int event)
{
	if (pm == NULL)
		return;

	/* �Ѱ����¼����ݵ����� */
	if (pm->page_list[pm->cur_page].page_event_handle != NULL)
		pm->page_list[pm->cur_page].page_event_handle(obj, event);
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

		printf("page change, old = %d, new = %d\n", pm->old_page, pm->new_page);
		printf("pm->page_list[pm->old_page].page_exit = %p\n", pm->page_list[pm->old_page].page_exit);

		/* �����ɽ����˳��¼� */
		if (CHECK_PAGE_ID(pm->old_page) && pm->page_list[pm->old_page].page_exit != NULL)
		{
			pm->page_list[pm->old_page].page_exit();
		}

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
