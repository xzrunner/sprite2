#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _sprite2_wrap_c_h_
#define _sprite2_wrap_c_h_

void  s2_on_size(int w, int h);

void  s2_render_ctx_push(float w, float h);
void  s2_render_ctx_pop();

/************************************************************************/
/* symbol                                                               */
/************************************************************************/



/************************************************************************/
/* sprite                                                               */
/************************************************************************/

int   s2_get_spr_count();

void  s2_spr_draw(const void* spr, float x, float y, float angle, float sx, float sy);
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

bool  s2_spr_get_visible(void* spr);
void  s2_spr_set_visible(void* spr, bool visible);
bool  s2_spr_get_editable(void* spr);
void  s2_spr_set_editable(void* spr, bool editable);

void  s2_spr_set_frame(void* spr, int frame);
void  s2_spr_set_action(void* spr, const char* action);

int   s2_spr_get_frame(void* spr);
int   s2_spr_get_frame_count(void* spr);
int   s2_spr_get_component_count(void* spr);

uint32_t s2_spr_get_col_mul(void* spr);
uint32_t s2_spr_get_col_add(void* spr);
void  s2_spr_get_col_map(void* spr, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap);

void  s2_spr_set_col_mul(void* spr, uint32_t rgba);
void  s2_spr_set_col_add(void* spr, uint32_t rgba);
void  s2_spr_set_col_map(void* spr, uint32_t rmap, uint32_t gmap, uint32_t bmap);
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

/************************************************************************/
/* actor                                                                */
/************************************************************************/

void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]);

void* s2_get_actor(const void* parent_actor, void* child_spr);
int   s2_get_actor_count();

void  s2_actor_mount_child(void* parent, const char* name, void* child);

void* s2_actor_get_spr(void* actor);
void  s2_actor_set_text(void* actor, const char* text);
void  s2_actor_print_path(void* actor);

void  s2_actor_set_pos(void* actor, float x, float y);
void  s2_actor_set_angle(void* actor, float angle);
void  s2_actor_set_scale(void* actor, float sx, float sy);

void* s2_actor_get_parent(void* actor);

#endif // _sprite2_wrap_c_h_

#ifdef __cplusplus
}
#endif