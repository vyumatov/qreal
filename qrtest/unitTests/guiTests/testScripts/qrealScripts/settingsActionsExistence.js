/* Copyright 2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

// For autocompletion and syntax highlighting
var mainWindow, ui, utils, keyboard, palette, hints, scene, cursor;

var menuSettings = ui.findMenu("menuSettings");
assert(menuSettings != null);

utils.activateMenu(menuSettings);
api.wait(200);

var actionPreferences = ui.findActionInMenu(menuSettings, "actionPreferences");
checkAction(actionPreferences, true, false, false);
expect(!ui.isSubMenuInMenu(menuSettings, actionPreferences));

function expectAction(actionName, isChecked) {
    var action = ui.findActionInMenu(menuSettings, actionName);
    checkAction(action, true, true, isChecked);
    expect(!ui.isSubMenuInMenu(menuSettings, action));
}

expectAction("actionShow_grid", true);
expectAction("actionShow_alignment", false);
expectAction("actionSwitch_on_grid", true);
expectAction("actionSwitch_on_alignment", false);
expectAction("actionShow_all_text", true);