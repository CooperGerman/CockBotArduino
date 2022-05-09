#include <Servo.h>
#include <gen_pwm.h>

Servo myservofull; // création de l'objet myservo
Servo myservozero; // création de l'objet myservo

gen_pwm_C::gen_pwm_C(unsigned pin_servofull, unsigned positionfull, unsigned pin_servozero, unsigned positionzero)
{
    _pin_servofull = pin_servofull;
    _positionfull = positionfull;
    _pin_servozero = pin_servozero;
    _positionzero = positionzero;
};

void gen_pwm_C::begin()
{
    myservofull.attach(_pin_servofull);
    myservofull.write(_positionfull);
    myservozero.attach(_pin_servozero);
    myservozero.write(_positionzero);
}

