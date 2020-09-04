#ifndef TOUCHGFXKEYS_HPP
#define TOUCHGFXKEYS_HPP

#include <platform/driver/button/ButtonController.hpp>
class TouchGFXKeys : public touchgfx::ButtonController
{
public:
    TouchGFXKeys()
    {
        init();
    }
    virtual ~TouchGFXKeys() {}
    virtual void init();
    virtual bool sample(uint8_t &key);

private:
    void encoderThread();
};

#endif /* TOUCHGFXKEYS_HPP */
