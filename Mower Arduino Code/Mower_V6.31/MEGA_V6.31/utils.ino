/**
 * Utils methods
 */

String dayAsString(const Time::Day day) {
    switch (day) {
        case Time::kSunday:
            return "Sunday";
        case Time::kMonday:
            return "Monday";
        case Time::kTuesday:
            return "Tuesday";
        case Time::kWednesday:
            return "Wednesday";
        case Time::kThursday:
            return "Thursday";
        case Time::kFriday:
            return "Friday";
        case Time::kSaturday:
            return "Saturday";
    }
    return "(unknown day)";
}