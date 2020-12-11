#ifndef CLASS_NCINE_RECTANIMATION
#define CLASS_NCINE_RECTANIMATION

#include <nctl/Array.h>
#include "Rect.h"

namespace ncine {

/// The class containing data for a rectangles based animation
class DLL_PUBLIC RectAnimation
{
  public:
	/// Loop modes
	enum class LoopMode
	{
		DISABLED,
		ENABLED
	};

	/// Rewind modes
	enum class RewindMode
	{
		/// When the animation reaches the last frame it begins again from start
		FROM_START,
		/// When the animation reaches the last frame it goes backward
		BACKWARD
	};

	/// Constructor for an animation with a specified default frame duration, loop and rewind mode
	RectAnimation(float defaultFrameDuration, LoopMode loopMode, RewindMode rewindMode);

	/// Returns true if the animation is paused
	inline bool isPaused() const { return isPaused_; }
	/// Sets the pause flag
	inline void setPaused(bool isPaused) { isPaused_ = isPaused; }

	/// Updates current frame based on time passed
	void updateFrame(float interval);

	/// Returns current frame
	inline unsigned int frame() const { return currentFrame_; }
	/// Sets current frame
	void setFrame(unsigned int frameNum);

	/// Returns the default frame duration in seconds
	float defaultFrameDuration() const { return defaultFrameDuration_; }
	/// Sets the default frame duration in seconds
	inline void setDefaultFrameDuration(float defaultFrameDuration) { defaultFrameDuration_ = defaultFrameDuration; }

	/// Adds a rectangle to the array specifying the frame duration
	void addRect(const Recti &rect, float frameTime);
	/// Creates a rectangle from origin and size and then adds it to the array, specifying the frame duration
	void addRect(int x, int y, int w, int h, float frameTime);

	/// Adds a rectangle to the array with the default frame duration
	inline void addRect(const Recti &rect) { addRect(rect, defaultFrameDuration_); }
	/// Creates a rectangle from origin and size and then adds it to the array, with the default frame duration
	inline void addRect(int x, int y, int w, int h) { addRect(x, y, w, h, defaultFrameDuration_); }

	/// Returns the current rectangle
	inline const Recti &rect() const { return rects_[currentFrame_]; }
	/// Returns the current frame duration in seconds
	inline float frameDuration() const { return frameDurations_[currentFrame_]; }

	/// Returns the array of all rectangles
	inline nctl::Array<Recti> &rectangles() { return rects_; }
	/// Returns the constant array of all rectangles
	inline const nctl::Array<Recti> &rectangles() const { return rects_; }

	/// Returns the array of all frame durations
	inline nctl::Array<float> &frameDurations() { return frameDurations_; }
	/// Returns the constant array of all frame durations
	inline const nctl::Array<float> &frameDurations() const { return frameDurations_; }

  private:
	/// The default frame duratin if a custom one is not specified when adding a rectangle
	float defaultFrameDuration_;
	/// The looping mode (on or off)
	LoopMode loopMode_;
	/// The rewind mode (ping-pong or not)
	RewindMode rewindMode_;

	/// The rectangles array
	nctl::Array<Recti> rects_;
	/// The frame durations array
	nctl::Array<float> frameDurations_;
	/// Current frame
	unsigned int currentFrame_;
	/// Elapsed time since the last frame change
	float elapsedFrameTime_;
	/// The flag about the frame advance direction
	bool goingForward_;
	/// The pause flag
	bool isPaused_;
};

}

#endif
