#ifndef _SPRITE2_ANIM_CURR_H_
#define _SPRITE2_ANIM_CURR_H_

#include "S2_Message.h"

#include <SM_Matrix.h>

#include <vector>

namespace s2
{

class Sprite;
class AnimSymbol;
class RenderParams;
class SprVisitor;

class AnimCurr
{
public:
	AnimCurr();
	AnimCurr(const AnimCurr& curr);
	AnimCurr& operator = (const AnimCurr& curr);
	AnimCurr(AnimSymbol* sym);
	~AnimCurr();

	bool Traverse(SprVisitor& visitor, const sm::mat4* mat = NULL) const;

	void OnMessage(Message msg);

	bool Update(const RenderParams& params, bool loop = true, 
		float interval = 0, int fps = 30);
	void Draw(const RenderParams& params) const;

	Sprite* FetchChild(const std::string& name) const;

	void Start();

	void SetTime(float time);
	void SetFrame(int frame, int fps);

	int GetFrame() const { return m_frame; }

	void SetActive(bool active);
	bool IsActive() const { return m_active; }

	void Clear();

private:
	void LoadFromSym();

	void ResetTime();

private:
	class Frame
	{
	public:
		Frame();
 		Frame(const Frame& f);
 		Frame& operator = (const Frame& f);
		~Frame();

		Sprite* Query(const Sprite* spr, int idx);

		void Clear();

	private:
		void CopyFrom(const std::vector<Sprite*>& src);

	public:
		std::vector<Sprite*> sprs;		

	}; // Frame

	class Layer
	{
	public:
		Frame frame;
	}; // Layer

private:
	AnimSymbol* m_sym;

	int m_frame;

	std::vector<Layer> m_layers;

	float m_start_time, m_curr_time;

	float m_stop_time, m_stop_during;

	bool m_active;

}; // AnimCurr

}

#endif // _SPRITE2_ANIM_CURR_H_