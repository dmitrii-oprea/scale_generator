#include "neckencoder.h"
#include <sstream>
#include <iostream>

std::string NeckEncoder::NeckToString(Neck &neck)
{
    std::string result;
    auto& strings = neck.GetStrings();

    for (int strNum = 0; strNum < (int)strings.size(); strNum++)
    {
        auto& string = strings[strNum];
        if (string.isMuted())
        {
            result += "x_";
        }
        else
        {
            bool isSelected = false;
            for (int i = 0; i < string.getFretNum(); i++)
            {
                if (string.isNoteSelected(i))
                {
                    isSelected = true;
                    result += std::to_string(i) + "_";
                    break;
                }
            }

            if (!isSelected)
            {
                result += "x_";
            }
        }
    }

    if (result.size() > 0)
    {
        result.resize(result.size() - 1);
    }
    return result;
}

Neck NeckEncoder::StringToNeck(const std::string &chordNotation)
{
    if (chordNotation.empty()) return Neck();
    std::vector<std::string> lines;
    std::istringstream ss(chordNotation);
    std::string s;
    while (std::getline(ss, s, '_'))
    {
        lines.push_back(s);
    }

    Neck result;
    auto& neckStrings = result.GetStrings();
    for (int i = 0; i < (int)lines.size(); i++)
    {
        const std::string& line = lines[i];
        if (i >= (int)neckStrings.size()) break;
        if (line == "x")
        {
            neckStrings[i].setMuted(true);
        }
        else
        {
            int num = std::stoi(line);
            neckStrings[i].SelectNote(num, true);
        }
    }

    return result;
}
