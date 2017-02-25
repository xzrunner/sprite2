#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _sprite2_wrap_c_h_
#define _sprite2_wrap_c_h_

#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

void s2_init();

void s2_on_size(int w, int h);

/************************************************************************/
/* symbol                                                               */
/************************************************************************/



/************************************************************************/
/* sprite                                                               */
/************************************************************************/

void  s2_spr_draw(const void* spr, float x, float y, float angle, float sx, float sy,
				  float xmin, float ymin, float xmax, float ymax);
void  s2_spr_update(void* spr);

void* s2_spr_fetch_child(const void* spr, const char* name);
void* s2_spr_fetch_child_by_index(const void* spr, int idx);

void  s2_spr_set_pos(void* spr, float x, float y);
void  s2_spr_set_angle(void* spr, float angle);
void  s2_spr_set_scale(void* spr, float sx, float sy);

void  s2_spr_get_pos(const void* spr, float* x, float* y);
void  s2_spr_get_angle(const void* spr, float* angle);
void  s2_spr_get_scale(const void* spr, float* sx, float* sy);

void  s2_spr_retain(void* spr);
void  s2_spr_release(void* spr);
int   s2_spr_get_id(void* spr);
int   s2_spr_get_ref_count(void* spr);
const char* s2_spr_get_name(void* spr);

int   s2_spr_get_sym_id(void* spr);
int   s2_spr_get_sym_type(void* spr);

bool  s2_spr_get_editable(void* spr);
void  s2_spr_set_editable(void* spr, bool editable);

bool  s2_spr_get_force_up_frame(void* spr);
void  s2_spr_set_force_up_frame(void* spr, bool force);

void  s2_spr_set_frame(void* spr, int frame);
void  s2_spr_set_action(void* spr, const char* action);

int   s2_spr_get_frame(void* spr);
int   s2_spr_get_frame_count(void* spr);
int   s2_spr_get_component_count(void* spr);

void  s2_spr_set_filter(void* spr, int mode);

const char* s2_spr_get_text(void* spr);
void  s2_spr_set_text(void* spr, const char* text);

void  s2_spr_get_aabb(const void* spr, float aabb[4]);
void  s2_spr_draw_aabb(const void* spr, float x, float y, float angle, float sx, float sy, const float mat[6]);
bool  s2_spr_point_test(const void* spr, float x, float y);
void* s2_spr_point_query(const void* spr, float x, float y, float mat[6]);

bool  s2_spr_has_action(const void* spr, const char* name);

bool  s2_spr_get_text_size(const void* spr, float* w, float* h);

bool  s2_spr_get_scissor(const void* spr, float* x, float* y, float* w, float* h);
bool  s2_spr_set_scissor(void* spr, float x, float y, float w, float h);

void  s2_spr_scale9_resize(void* spr, int w, int h);

/************************************************************************/
/* actor                                                                */
/************************************************************************/

void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]);

void* s2_get_actor(const void* parent_actor, void* child_spr);
int   s2_get_actor_count();

void  s2_actor_set_proxy(void* actor, void* spr);

void* s2_actor_get_spr(void* actor);
void  s2_actor_set_text(void* actor, const char* text);
void  s2_actor_print_path(void* actor);

void  s2_actor_set_pos(void* actor, float x, float y);
void  s2_actor_set_angle(void* actor, float angle);
void  s2_actor_set_scale(void* actor, float sx, float sy);

void* s2_actor_get_parent(void* actor);

bool  s2_actor_get_visible(void* actor);
void  s2_actor_set_visible(void* actor, bool visible);

uint32_t s2_actor_get_col_mul(void* actor);
void     s2_actor_set_col_mul(void* actor, uint32_t rgba);
uint32_t s2_actor_get_col_add(void* actor);
void     s2_actor_set_col_add(void* actor, uint32_t rgba);
void     s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap);
void     s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap);

/************************************************************************/
/* rt                                                                   */
/************************************************************************/

struct rect_tex {
	float xmin, ymin;
	float xmax, ymax;
};

void* s2_rt_fetch();
void  s2_rt_return(void* rt);
void  s2_rt_draw_from(void* rt, const struct rect_tex* dst, const struct rect_tex* src, int src_tex_id);
void  s2_rt_draw_to(void* rt, const struct rect_tex* dst, const struct rect_tex* src);
int   s2_rt_get_texid(void* rt);

/************************************************************************/
/* camera                                                               */
/************************************************************************/

void* s2_cam_create();
void  s2_cam_release(void* cam);
void  s2_cam_bind(const void* cam);
void  s2_cam_set(void* cam, float x, float y, float scale);

#endif // _sprite2_wrap_c_h_

#ifdef __cplusplus
}
#endif