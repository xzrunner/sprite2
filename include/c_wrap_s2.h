#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _sprite2_wrap_c_h_
#define _sprite2_wrap_c_h_

void  s2_on_size(int w, int h);

void  s2_render_ctx_push(float w, float h);
void  s2_render_ctx_pop();

void  s2_spr_retain(void* spr);
void  s2_spr_release(void* spr);

void* s2_get_actor(const void* parent_actor, void* child_spr);

int   s2_get_actor_count();

void* s2_actor_get_spr(void* actor);
void  s2_actor_set_text(void* actor, const char* text);

#endif // _sprite2_wrap_c_h_

#ifdef __cplusplus
}
#endif