#ifndef FLOATING_EYE_STATE_H
#define FLOATING_EYE_STATE_H

#include <string>
#include <tuple>

class State
{
public:
    State() = default;
    State(const std::string& line1, const std::string& line2);
    virtual ~State() = default;

    void update(const std::string& line1, const std::string& line2);
    bool isValid() const { return valid_; }

private:
    bool valid_ = false;

    /// https://en.wikipedia.org/wiki/Two-line_element_set
    using TLE_Line1 = std::tuple<
    int,    // Line number
    int,    // Satellite number
    char,   // Classification (U=Unclassified)
    int,    // International Designator (Last two digits of launch year)
    int,    // International Designator (Launch number of the year)
    char,   // International Designator (piece of the launch)
    int,    // Epoch Year (last two digits of year)
    float,  // Epoch (day of the year and fractional portion of the day)
    double, // First Time Derivative of the Mean Motion divided by two
    double, // Second Time Derivative of Mean Motion divided by six (decimal point assumed)
    double, // BSTAR drag term (decimal point assumed)
    int,    // The number 0 (originally this should have been "Ephemeris type")
    long long, // Element set number. Incremented when a new TLE is generated for this object.
    int>;      // Checksum (modulo 10)

    using TLE_Line2 = std::tuple<
        int,    // Line number
        int,    // Satellite number
        double, // Inclination (degrees)
        double, // Right ascension of the ascending node (degrees)
        double, // Eccentricity (decimal point assumed)
        double, // Argument of perigee (degrees)
        double, // Mean Anomaly (degrees)
        double, // Mean Motion (revolutions per day)
        long long, // Revolution number at epoch (revolutions)
        int>;      // Checksum (modulo 10)

    TLE_Line1 line1_;
    TLE_Line2 line2_;
};

#endif // FLOATING_EYE_STATE_H
