#pragma once

#include <common.hpp>

#include <SDL.h>
#include <toml.hpp>


class Settings {
public:
    void load();
    void save();

    // --- The following functions must be called after load()---

    // Default settings

    struct {

        std::string nickname = "Alber";

    } system;

    struct {

        bool partialLv2LLE = true;
        bool sys_fs = false;
        bool cellResc = true;
        bool cellPngDec = false;
        bool cellFont = false;
        bool cellFontFT = false;
        bool cell_FreeType2 = false;
        bool cellSync = true;
        bool cellSpurs = true;
        bool cellSpursJq = true;
        bool cellKey2char = true;
        bool cellL10n = true;
        bool cellFiber = true;

        inline bool isLLEEnabled(std::string name) {
            if (name == "sys_fs")          return sys_fs;
            else if (name == "cellResc")        return cellResc;
            else if (name == "cellPngDec")      return cellPngDec;
            else if (name == "cellFont")        return cellFont;
            else if (name == "cellFontFT")      return cellFontFT;
            else if (name == "cell_FreeType2")  return cell_FreeType2;
            else if (name == "cellSync")        return cellSync;
            else if (name == "cellSpurs")       return cellSpurs;
            else if (name == "cellSpursJq")     return cellSpursJq;
            else if (name == "cellKey2char")    return cellKey2char;
            else if (name == "cellL10n")        return cellL10n;
            else if (name == "cellFiber")       return cellFiber;
            else Helpers::panic("Tried to get LLE configuration of unknown module %s\n", name.c_str());
        }

    } lle;
};