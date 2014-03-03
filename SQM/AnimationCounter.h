#pragma once

struct AnimationCounter {
	float counter;
	float speed;
	int frame;
	int nextFrame;
	int keyframes;
	bool backwards;
	bool cyclic;
	AnimationCounter() : counter(0), speed(0), frame(0), keyframes(1), backwards(false), cyclic(false) {};
	AnimationCounter(float _speed) : counter(0), speed(_speed), frame(0), nextFrame(0), keyframes(1), backwards(false), cyclic(false) {};
	AnimationCounter(float _speed, int _frame, int _keyframes) : counter(0), speed(_speed), frame(_frame), keyframes(_keyframes), backwards(false), cyclic(false) {nextFrame = frame + 1; if (nextFrame >= keyframes) nextFrame = 0;};
	AnimationCounter(float _counter, float _speed, int _frame, int _keyframes) : counter(_counter), speed(_speed), frame(_frame), keyframes(_keyframes), backwards(false), cyclic(false) {nextFrame = frame + 1; if (nextFrame >= keyframes) nextFrame = 0;};
	void Next() {
		if (counter > 1.0) {
			counter = 0;
			frame = nextFrame;
			nextFrame++;
			if (nextFrame == keyframes) {
				nextFrame = 0;
				if (cyclic) {
					backwards = true;
					frame = keyframes - 1;
					nextFrame = keyframes - 2;
				}
			}
		}
	}
	void Prev() {
		if (counter > 1.0) {
			counter = 0;
			frame = nextFrame;
			nextFrame--;
			if (nextFrame == -1) {
				nextFrame = keyframes-1;
				if (cyclic) {
					backwards = false;
					frame = 0;
					nextFrame = 1;
				}
			}
		}
	}
	void Animate() {
		counter += speed;
		if (backwards) Prev(); else Next();
	}
};