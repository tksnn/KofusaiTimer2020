#pragma once
#include <Siv3D.hpp>

class TimerS {
private:
	Stopwatch sw;
	SecondsF limit;
public:
	bool ex_flag = false;
	explicit TimerS(Duration limit_s, bool startImmediately = false) {
		limit = limit_s;
		ex_flag = false;
		if (startImmediately) sw.start();
	}
	void start() {
		ex_flag = false;
		sw.start();
	}
	[[nodiscard]] Duration duration() const { return limit; };
	[[nodiscard]] Duration remaining() const {
		return limit - sw.elapsed();
	};
	[[nodiscard]] double sF() const {
		return limit.count() - sw.sF();
	}
	[[nodiscard]] int32 s() const {
		return static_cast<int32>(sF());
	}
	[[nodiscard]] String mmss() const {
		int second = static_cast<int>(sF());
		double dsecond = sF();
		if (dsecond > 0) {
			int m = second / 60;
			int s = second % 60;
			if (m >= 10) {
				if (s >= 10) {
					return Format(m, U":", s);
				}
				else {
					return Format(m, U":0", s);
				}
			}
			else {
				if (s >= 10) {
					return Format(U"0", m, U":", s);
				}
				else {
					return Format(U"0", m, U":0", s);
				}
			}
		}
		else {
			int m_second = static_cast<int>(sw.sF() - limit.count());
			int m = m_second / 60;
			int s = m_second % 60 + 1;
			if (m >= 10) {
				if (s >= 10) {
					return Format(U"-", m, U":", s);
				}
				else {
					return Format(U"-", m, U":0", s);
				}
			}
			else {
				if (s >= 10) {
					return Format(U"-", m, U":", s);
				}
				else {
					return Format(U"-", m, U":0", s);
				}
			}
		}
	}
	[[nodiscard]] bool isStarted() const { return sw.isStarted(); };
	[[nodiscard]] bool isPaused() const { return sw.isPaused(); };
	[[nodiscard]] bool isRunning() const { return sw.isRunning(); };
	/*タイマーがゼロに行ってるかどうか*/
	[[nodiscard]] bool reachedZero() const {
		return (sw.elapsed() >= limit);
	}
	void pause() {
		sw.pause();
	}
	void remuse() {
		if (!isPaused()) {
			return;
		}
		start();
	}
	void restart() {
		sw.restart();
	}
	void restart(Duration time) {
		limit = time;
		sw.restart();
	}
	void reset() {
		ex_flag = false;
		sw.reset();
	}
	void reset(Duration time) {
		limit = time;
		sw.reset();
	}
	void set(Duration time) {
		limit = time;
	}
	void add(Duration time) {
		limit += time;
	}
};