/*
 *   This file is part of Checkpoint
 *   Copyright (C) 2017-2019 Bernardo Giordano, FlagBrew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#include "gui.hpp"

void Gui::drawCopy(const std::string& src, u64 offset, u64 size)
{
    std::string sizeString = StringUtils::sizeString(offset) + " of " + StringUtils::sizeString(size);
    u32 srcw, srch, textw, texth;
    SDLH_GetTextDimensions(28, src.c_str(), &srcw, &srch);
    SDLH_GetTextDimensions(26, sizeString.c_str(), &textw, &texth);
    SDLH_DrawRect(320, 200, 640, 240, theme().c3);
    SDLH_DrawRect(320, 440, 640, 80, theme().c2);
    SDLH_DrawRect(320, 440, (float)offset / (float)size * 640, 80, COLOR_BLUE);
    SDLH_DrawText(28, ceilf((1280 - srcw) / 2), 200 + ceilf((220 - srch) / 2), theme().c6, src.c_str());
    SDLH_DrawText(26, ceilf((1280 - textw) / 2), 440 + ceilf((80 - texth) / 2), theme().c6, sizeString.c_str());
    SDLH_Render();
}

bool Gui::askForConfirmation(const std::string& text)
{
    bool ret             = false;
    Clickable* buttonYes = new Clickable(322, 462, 316, 56, theme().c3, theme().c6, "Yes", true);
    Clickable* buttonNo  = new Clickable(642, 462, 316, 56, theme().c3, theme().c6, "No", true);
    HidHorizontal* mhid  = new HidHorizontal(2, 2);

    u32 w, h;
    SDLH_GetTextDimensions(28, text.c_str(), &w, &h);

    while (appletMainLoop()) {
        hidScanInput();
        mhid->update(2);

        if (buttonYes->released() || ((hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) && mhid->index() == 0)) {
            ret = true;
            break;
        }
        else if (buttonNo->released() || (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_B) ||
                 ((hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) && mhid->index() == 1)) {
            break;
        }

        mhid->index(buttonYes->held() ? 0 : buttonNo->held() ? 1 : mhid->index());
        buttonYes->selected(mhid->index() == 0);
        buttonNo->selected(mhid->index() == 1);

        SDLH_DrawRect(320, 200, 640, 260, theme().c3);
        SDLH_DrawText(28, ceilf(1280 - w) / 2, 200 + ceilf((260 - h) / 2), theme().c6, text.c_str());
        drawOutline(322, 462, 316, 56, 2, theme().c5);
        drawOutline(642, 462, 316, 56, 2, theme().c5);
        buttonYes->draw(28, COLOR_BLUE);
        buttonNo->draw(28, COLOR_BLUE);

        if (mhid->index() == 0) {
            drawPulsingOutline(324, 464, 312, 52, 4, COLOR_BLUE);
        }
        else {
            drawPulsingOutline(644, 464, 312, 52, 4, COLOR_BLUE);
        }

        SDLH_Render();
    }

    delete mhid;
    delete buttonYes;
    delete buttonNo;
    return ret;
}

void Gui::showInfo(const std::string& message)
{
    Clickable* button = new Clickable(322, 462, 636, 56, theme().c3, theme().c6, "OK", true);
    button->selected(true);
    u32 w, h;
    SDLH_GetTextDimensions(28, message.c_str(), &w, &h);

    while (appletMainLoop()) {
        hidScanInput();

        if (button->released() || (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) || (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_B)) {
            break;
        }

        SDLH_DrawRect(320, 200, 640, 260, theme().c3);
        SDLH_DrawText(28, ceilf(1280 - w) / 2, 200 + ceilf((260 - h) / 2), theme().c6, message.c_str());
        button->draw(28, COLOR_BLUE);
        drawPulsingOutline(322, 462, 636, 56, 4, COLOR_BLUE);

        SDLH_Render();
    }

    delete button;
}

void Gui::showError(Result res, const std::string& message)
{
    Clickable* button = new Clickable(322, 462, 636, 56, theme().c1, theme().c6, "OK", true);
    button->selected(true);
    u32 w, h;
    SDLH_GetTextDimensions(28, message.c_str(), &w, &h);

    while (appletMainLoop()) {
        hidScanInput();

        if (button->released() || (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) || (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_B)) {
            break;
        }

        SDLH_DrawRect(320, 200, 640, 260, theme().c0);
        SDLH_DrawText(20, 330, 210, COLOR_RED, StringUtils::format("Error: 0x%0llX", res).c_str());
        SDLH_DrawText(28, ceilf(1280 - w) / 2, 200 + ceilf((260 - h) / 2), theme().c6, message.c_str());
        button->draw(28, COLOR_RED);
        drawPulsingOutline(322, 462, 636, 56, 4, COLOR_RED);

        SDLH_Render();
    }

    delete button;
}