#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _sprite2_wrap_c_h_
#define _sprite2_wrap_c_h_

void s2_on_size(int w, int h);

void s2_render_ctx_push(float w, float h);
void s2_render_ctx_pop();

#endif // _sprite2_wrap_c_h_

#ifdef __cplusplus
}
#endif