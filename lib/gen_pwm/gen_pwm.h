#ifndef gen_pwm_h
#define gen_pwm_h

class gen_pwm_C
{
    public:
        gen_pwm_C(unsigned pin_servofull, unsigned positionfull, unsigned pin_servozero, unsigned positionzero);    //
        void begin();    //

    protected:
        unsigned _pin_servofull;
        unsigned _positionfull;
        unsigned _pin_servozero;
        unsigned _positionzero;
};
#endif