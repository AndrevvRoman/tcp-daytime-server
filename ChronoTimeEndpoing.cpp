#include "ChronoTimeEndpoing.hpp"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <algorithm>

class ChronoTimeEndpoint::pImpl
{
public:
    pImpl(
        const std::string &format, std::function<time_t()> timeGetter = []()
                                   { return std::time(nullptr); })
        : m_format(format),
          m_timeGetter(timeGetter)
    {
    }

    std::pair<std::string, bool> getNowInTimezone(const std::string &timeZone)
    {
        std::time_t now = m_timeGetter();
        std::time_t gmtNow = std::mktime(std::gmtime(&now));
        auto chronoTimeInGmt = std::chrono::system_clock::from_time_t(gmtNow);

        std::string timeZoneUpper = _toUpper(timeZone);
        try
        {
            auto timeInTargetTimeZone = chronoTimeInGmt + std::chrono::hours(m_timeShiftsMap.at(timeZoneUpper));
            auto targetTime = std::chrono::system_clock::to_time_t(timeInTargetTimeZone);
            std::tm targetTimeStruct = *std::localtime(&targetTime);
            targetTimeStruct.tm_zone = timeZoneUpper.c_str();
            std::stringstream ss;
            ss << std::put_time(&targetTimeStruct, m_format.c_str());
            return {ss.str(), true};
        }
        catch (const std::out_of_range &e)
        {
            return {"", false};
        }
    }
    void setFormat(const std::string &format)
    {
        m_format = format;
    }

private:
    std::function<time_t()> m_timeGetter;
    std::string _toUpper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
        return str;
    }
    std::string m_format = "%a %b %d %T %Z %Y";
    const std::map<std::string, int> m_timeShiftsMap{
        {"GMT", 0},
        {"IBST", 0},
        {"WET", 0},
        {"Z", 0},
        {"EGST", 0},
        {"BST", 1},
        {"CET", 1},
        {"DFT", 1},
        {"IST", 1},
        {"MET", 1},
        {"WAT", 1},
        {"WEDT", 1},
        {"WEST", 1},
        {"CAT", 2},
        {"CEDT", 2},
        {"CEST", 2},
        {"EET", 2},
        {"HAEC", 2},
        {"IST", 2},
        {"MEST", 2},
        {"SAST", 2},
        {"USZ1", 2},
        {"WAST", 2},
        {"AST", 3},
        {"EAT", 3},
        {"EEDT", 3},
        {"EEST", 3},
        {"FET", 3},
        {"IDT", 3},
        {"IOT", 3},
        {"MSK", 3},
        {"SYOT", 3},
        {"AMT", 4},
        {"AZT", 4},
        {"GET", 4},
        {"GST", 4},
        {"MUT", 4},
        {"RET", 4},
        {"SAMT", 4},
        {"SCT", 4},
        {"VOLT", 4},
        {"HMT", 5},
        {"MAWT", 5},
        {"MVT", 5},
        {"ORAT", 5},
        {"PKT", 5},
        {"TFT", 5},
        {"TJT", 5},
        {"TMT", 5},
        {"UZT", 5},
        {"YEKT", 5},
        {"BDT", 6},
        {"BIOT", 6},
        {"BST", 6},
        {"BTT", 6},
        {"KGT", 6},
        {"OMST", 6},
        {"VOST", 6},
        {"CXT", 7},
        {"DAVT", 7},
        {"HOVT", 7},
        {"ICT", 7},
        {"KRAT", 7},
        {"THA", 7},
        {"WIT", 7},
        {"ACT", 8},
        {"AWST", 8},
        {"BDT", 8},
        {"CHOT", 8},
        {"CIT", 8},
        {"CST", 8},
        {"CT", 8},
        {"HKT", 8},
        {"IRKT", 8},
        {"MST", 8},
        {"MYT", 8},
        {"PST", 8},
        {"SGT", 8},
        {"SST", 8},
        {"ULAT", 8},
        {"WST", 8},
        {"AWDT", 9},
        {"EIT", 9},
        {"JST", 9},
        {"KST", 9},
        {"TLT", 9},
        {"YAKT", 9},
        {"AEST", 10},
        {"ChST", 10},
        {"CHUT", 10},
        {"DDUT", 10},
        {"EST", 10},
        {"PGT", 10},
        {"VLAT", 10},
        {"AEDT", 11},
        {"BST", 11},
        {"KOST", 11},
        {"LHST", 11},
        {"MIST", 11},
        {"NCT", 11},
        {"PONT", 11},
        {"SAKT", 11},
        {"SBT", 11},
        {"SRET", 11},
        {"VUT", 11},
        {"FJT", 12},
        {"GILT", 12},
        {"MAGT", 12},
        {"MHT", 12},
        {"NZST", 12},
        {"PETT", 12},
        {"TVT", 12},
        {"WAKT", 12},
        {"NZDT", 13},
        {"PHOT", 13},
        {"TKT", 13},
        {"TOT", 13},
        {"LINT", 14},
        {"AZOST", -1},
        {"CVT", -1},
        {"EGT", -1},
        {"BRST", -2},
        {"FNT", -2},
        {"GST", -2},
        {"PMDT", -2},
        {"UYST", -2},
        {"ADT", -3},
        {"AMST", -3},
        {"ART", -3},
        {"BRT", -3},
        {"CLST", -3},
        {"FKST", -3},
        {"FKST", -3},
        {"GFT", -3},
        {"PMST", -3},
        {"PYST", -3},
        {"ROTT", -3},
        {"SRT", -3},
        {"UYT", -3},
        {"AMT", -4},
        {"AST", -4},
        {"BOT", -4},
        {"CDT", -4},
        {"CLT", -4},
        {"COST", -4},
        {"ECT", -4},
        {"EDT", -4},
        {"FKT", -4},
        {"GYT", -4},
        {"PYT", -4},
        {"ACT", -5},
        {"CDT", -5},
        {"COT", -5},
        {"CST", -5},
        {"EASST", -5},
        {"ECT", -5},
        {"EST", -5},
        {"PET", -5},
        {"CST", -6},
        {"EAST", -6},
        {"GALT", -6},
        {"MDT", -6},
        {"MST", -7},
        {"PDT", -7},
        {"AKDT", -8},
        {"CIST", -8},
        {"PST", -8},
        {"AKST", -9},
        {"GAMT", -9},
        {"GIT", -9},
        {"HADT", -9},
        {"CKT", -10},
        {"HAST", -10},
        {"HST", -10},
        {"TAHT", -10},
        {"NUT", -11},
        {"SST", -11},
        {"BIT", -12}};
};

ChronoTimeEndpoint::ChronoTimeEndpoint(
    const std::string &format, std::function<time_t()> timeGetter)
    : m_pImpl(new pImpl(format,timeGetter))
{
}

std::pair<std::string, bool> ChronoTimeEndpoint::getNowInTimezone(const std::string &timeZone)
{
    return m_pImpl->getNowInTimezone(timeZone);
}
void ChronoTimeEndpoint::setFormat(const std::string &format)
{
    m_pImpl->setFormat(format);
}

ChronoTimeEndpoint::~ChronoTimeEndpoint()
{
    if (m_pImpl != nullptr)
    {
        delete m_pImpl;
    }
}
