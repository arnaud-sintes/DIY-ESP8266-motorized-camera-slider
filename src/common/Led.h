#pragma once


class Led
{
public:
    Led();
    ~Led() = default;

public:
    const bool Enabled() const { return m_enabled; }
    void Enable( const bool _enable = true );
    void Disable();
    void Toggle();

private:
    bool m_enabled{ false };
};