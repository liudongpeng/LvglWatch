//
// Created by QT on 2023/1/28.
//

#ifndef HARDWARE_TEST_PAGE_H
#define HARDWARE_TEST_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stddef.h>


/**
 * @brief
 */
typedef struct page
{
	uint8_t page_id;    /* 界面id */

	struct page* page_addr; /* 界面对象地址 */

	/**
	 * @brief 设置界面
	 */
	void (*page_setup)();

	/**
	 * @brief 界面后台运行
	 */
	void (*page_loop)();

	/**
	 * @brief 退出界面
	 */
	void (*page_exit)();

	/**
	 * @brief 界面事件处理
	 * @param[in]	obj
	 * @param[in]	event
	 */
	void (*page_event_handle)(void *obj, int event);

} page_t;


/**
 * @brief 界面管理器
 */
typedef struct page_manager
{
	page_t *page_list;  /* 界面列表 */


	uint8_t *page_stack;    /* 界面管理栈 */
	uint8_t last_page, cur_page, next_page;
	uint8_t new_page, old_page;
	uint8_t page_stack_size, max_page_num;
	int8_t page_stack_top;

	uint8_t is_busy;    /* 是否繁忙 */
} page_manager_t;


typedef void (*page_cb_t)();
typedef void (*page_event_cb_t)(void *, int);


int page_manager_create(page_manager_t *pm, uint8_t max_page_num, uint8_t page_stack_size);

int page_create(page_t *page, page_cb_t page_setup, page_cb_t page_loop, page_cb_t page_exit,
                page_event_cb_t page_event_handle);

int page_register(page_manager_t *pm, uint8_t id, page_cb_t page_setup, page_cb_t page_loop, page_cb_t page_exit,
                  page_event_cb_t page_event_handle);

void page_clear(page_manager_t *pm, uint8_t id);

void page_stack_clear(page_manager_t *pm);

int page_push(page_manager_t *pm, uint8_t id);
int page_push_by_id(page_manager_t *pm, uint8_t id);

int page_pop(page_manager_t *pm);

int page_change(page_manager_t *pm, uint8_t id);

void page_event_transmit(page_manager_t *pm, void* obj, int event);

void page_run(page_manager_t *pm);


page_t* get_page_by_id(page_manager_t* pm, uint8_t id);


#ifdef __cplusplus
}
#endif

#endif //HARDWARE_TEST_PAGE_H
