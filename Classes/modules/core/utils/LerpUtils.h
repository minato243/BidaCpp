#ifndef LERPUTILS_H_
#define LERPUTILS_H_

#include <modules/core/ExtensionMath.hpp>
#include <functional>
#include "cocos2d.h"
#include <data/DataStruct.h>

extern const double NUMBER_LERP_STOP_RPT;

struct LerpListener {
	std::function<void()> handler;
};

template<typename T>
class BaseLerp
{
public:
	virtual void dispatch() {};
	void setEnabled(bool enabled) { _enabled = enabled; };
	void setCurent(T current) { _current = current; };
	T getCurrent() { return _current; };
protected:
	bool _enabled = false;
	T _current;
};

class SphericalLerp : public BaseLerp<Rpt>{
public:
	SphericalLerp(double lerp, int context, std::function<void(Rpt)> listener);
	~SphericalLerp();
	void setCurrent(Rpt current);
	void setTarget(Rpt target);
	void fastForward();
	void dispatch();
	void update(float dt);
	void simplifyRadian();
	Rpt getCurrent();
	void onceCompleted(std::function<void()> callback);
	static SphericalLerp* makeLerp(double lerp, int context, std::function<void(Rpt)> listener);

private:
	double _lerp;
	Rpt _current;
	Rpt _target;
	bool _enabled;
	std::function<void(Rpt)> _listener;
	int _context;
	LerpListener *_completedListener;
};

class VectorLerp: public BaseLerp<ps::ExtMath::vector> {
public:
	VectorLerp(double lerp, std::function<void(ps::ExtMath::vector)> listener);
	~VectorLerp();
	void setCurrent(ps::ExtMath::vector current);
	void setTarget(ps::ExtMath::vector current);
	void fastForward();
	virtual void dispatch();
	void update(float dt);
	ps::ExtMath::vector getCurrent();
	void onceCompleted(std::function<void()> callback);

private:
	double _lerp;
	ps::ExtMath::vector _current;
	ps::ExtMath::vector _target;
	
	std::function<void(ps::ExtMath::vector)> _listener;
	int _context;
	LerpListener* _completedListener;
};

class NumberLerp :public BaseLerp<double>{
public:
	NumberLerp(double lerp, std::function<void(double)> listener);
	~NumberLerp();

	void setCurrent(double current);
	void setTarget(double target);
	void fastForward();
	void dispatch();
	void update(float dt);
	static NumberLerp* makeLerp();
private:
	double _lerp;
	double _current;
	double _target;
	bool _enabled = false;
	std::function<void(double)> _listener;
	int _context;
};

class RPTOLerp{
public:
	RPTOLerp();
	~RPTOLerp();
	void setCurrent(Rpt currentRPT, ps::ExtMath::vector currentOff);
	void setTarget(Rpt currentRPT, ps::ExtMath::vector currentOff);
	void fastForward();
	void dispatch();
	void update(float dt);
	void handleRPT(Rpt value);
	void handleOff(ps::ExtMath::vector value);
	void simplifyRadian();
	void onceCompleted();
	void clearOnceCompletedListener();
	void _elementOnceCompleted();
	static RPTOLerp* makeLerp();
private:
	Rpt _resultRPT;
	ps::ExtMath::vector _resultOff;

	SphericalLerp* _rptLerp;
	VectorLerp* _offLerp;

	int _context;
	int _listener;
	bool _enabled = false;
	int _completedListener;
};

#endif // !LERPUTILS_H_
