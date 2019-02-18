#include "main.h"

int autonMain = 0;
int autonNearFar = 0;

lv_obj_t * autonSelectPageTitle = lv_label_create(autonSelectPage, NULL);

lv_obj_t * tabContainer;

lv_obj_t * autonNone;
lv_obj_t * autonRed;
lv_obj_t * autonBlue;
lv_obj_t * autonLaunch;

lv_obj_t * autonNonePage;
lv_obj_t * autonNoneDesc;

lv_obj_t * autonSidePage;
lv_obj_t * autonSidePlatform;
lv_obj_t * autonSideNear;
lv_obj_t * autonSideNearPage;
lv_obj_t * autonSideNearDesc;
lv_obj_t * autonSideFar;
lv_obj_t * autonSideFarPage;
lv_obj_t * autonSideFarDesc;

lv_obj_t * autonLaunchPage;
lv_obj_t * autonLaunchSpeed;
lv_obj_t * autonLaunchSlider;
lv_obj_t * autonLaunchDesc;

void btnSetToggled(lv_obj_t * btn, bool toggled)
{
    if(toggled != (lv_btn_get_state(btn) >= 2)) lv_btn_toggle(btn);
}

lv_style_t * createBtnStyle(lv_style_t * copy, lv_color_t rel, lv_color_t pr,
    lv_color_t tglRel, lv_color_t tglPr, lv_color_t tglBorder, lv_color_t textColor, lv_obj_t * btn)
{
    lv_style_t * btnStyle = (lv_style_t *)malloc(sizeof(lv_style_t) * 4);

    for(int i = 0; i < 4; i++) lv_style_copy(&btnStyle[i], copy);

    btnStyle[0].body.main_color = rel;
    btnStyle[0].body.grad_color = rel;
    btnStyle[0].text.color = textColor;

    btnStyle[1].body.main_color = pr;
    btnStyle[1].body.grad_color = pr;
    btnStyle[1].text.color = textColor;

    btnStyle[2].body.main_color = tglRel;
    btnStyle[2].body.grad_color = tglRel;
    btnStyle[2].body.border.width = 2;
    btnStyle[2].body.border.color = tglBorder;
    btnStyle[2].text.color = textColor;

    btnStyle[3].body.main_color = tglPr;
    btnStyle[3].body.grad_color = tglPr;
    btnStyle[3].body.border.width = 2;
    btnStyle[3].body.border.color = tglBorder;
    btnStyle[3].text.color = textColor;

    lv_btn_set_style(btn, LV_BTN_STYLE_REL, &btnStyle[0]);
    lv_btn_set_style(btn, LV_BTN_STYLE_PR, &btnStyle[1]);
    lv_btn_set_style(btn, LV_BTN_STYLE_TGL_REL, &btnStyle[2]);
    lv_btn_set_style(btn, LV_BTN_STYLE_TGL_PR, &btnStyle[3]);

    return btnStyle;
}

void setBtnStyle(lv_style_t * btnStyle, lv_obj_t * btn)
{
    lv_btn_set_style(btn, LV_BTN_STYLE_REL, &btnStyle[0]);
    lv_btn_set_style(btn, LV_BTN_STYLE_PR, &btnStyle[1]);
    lv_btn_set_style(btn, LV_BTN_STYLE_TGL_REL, &btnStyle[2]);
    lv_btn_set_style(btn, LV_BTN_STYLE_TGL_PR, &btnStyle[3]);
}

lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
    int id, const char * title)
{
    lv_obj_t * btn = lv_btn_create(parent, NULL);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, width, height);
    lv_obj_set_free_num(btn, id);

    lv_obj_t * label = lv_label_create(btn, NULL);
    lv_label_set_text(label, title);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    return btn;
}

static lv_res_t btn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    if(id == 100)
    {
        lv_obj_set_hidden(autonSelectPage, true);
        lv_obj_set_hidden(textViewPage, false);
        lv_obj_set_hidden(autonPageButton, false);
        return LV_RES_OK;
    }
    if(id == 101)
    {
        lv_obj_set_hidden(autonSelectPage, false);
        lv_obj_set_hidden(textViewPage, true);
        lv_obj_set_hidden(autonSelectPageCloseToTextView, false);
        lv_obj_set_hidden(autonRun, false);
        return LV_RES_OK;
    }
    if(id == 102)
    {
        autonDriverControl = true;
        return LV_RES_OK;
    }

    if(id == 0 || id == 1 || id == 2 || id == 3)
    {
        autonMain = id;

        btnSetToggled(autonNone, autonMain == 0);
        btnSetToggled(autonRed, autonMain == 1);
        btnSetToggled(autonBlue, autonMain == 2);
        btnSetToggled(autonLaunch, autonMain == 3);

        lv_obj_set_hidden(autonNonePage, autonMain != 0);
        lv_obj_set_hidden(autonSidePage, autonMain != 1 && autonMain != 2);
        lv_obj_set_hidden(autonLaunchPage, autonMain != 3);
    }

    if(id == 4 || id == 5)
    {
        autonNearFar = id - 4;

        btnSetToggled(autonSideNear, id == 4);
        btnSetToggled(autonSideFar, id == 5);

        lv_obj_set_hidden(autonSideNearPage, id != 4);
        lv_obj_set_hidden(autonSideFarPage, id != 5);
    }

    if(id == 6)
    {
        autonPlatform++;
        if(autonPlatform == 3) autonPlatform = 0;
        //driveOntoPlatform = !driveOntoPlatform;
        //btnSetToggled(autonSidePlatform, driveOntoPlatform);
        if(autonPlatform == 0)
        {
            lv_label_set_text(lv_obj_get_child(autonSidePlatform, NULL), "Platform: none");
            lv_label_set_text(autonSideNearDesc, "The robot will run autonomous on near tile.\nIt will get a second ball and flip all three flags.");
            lv_label_set_text(autonSideFarDesc, "The robot will run autonomous on far tile.\nIt will get a second ball and flip all two flags in the center.");
        }
        else if(autonPlatform == 1)
        {
            lv_label_set_text(lv_obj_get_child(autonSidePlatform, NULL), "Platform: low");
            lv_label_set_text(autonSideNearDesc, "The robot will run autonomous on near tile.\nIt will get a second ball, flip all three flags, \nand drive onto the low platform.");
            lv_label_set_text(autonSideFarDesc, "The robot will run autonomous on far tile.\nIt will get a second ball, flip all two flags in the center, \nand drive onto the low platform.");
        }
        else if(autonPlatform == 2)
        {
            lv_label_set_text(lv_obj_get_child(autonSidePlatform, NULL), "Platform: high");
            lv_label_set_text(autonSideNearDesc, "WARNING: The robot will run autonomous on near tile.\nIt will get a second ball, flip all three flags, \nand drive onto the high platform.");
            lv_label_set_text(autonSideFarDesc, "WARNING: The robot will run autonomous on far tile.\nIt will get a second ball, flip all two flags in the center, \nand drive onto the high platform.");
        }
    }

    if(autonMain == 0) autonomousPreSet = 0;
    if(autonMain == 1 && autonNearFar == 0) autonomousPreSet = 1;
    if(autonMain == 1 && autonNearFar == 1) autonomousPreSet = 2;
    if(autonMain == 2 && autonNearFar == 0) autonomousPreSet = 100;
    if(autonMain == 2 && autonNearFar == 1) autonomousPreSet = 101;
    if(autonMain == 3) autonomousPreSet = 200;

    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

static lv_res_t autonLaunchSlider_action(lv_obj_t * slider)
{
    char buffer[100];
    sprintf(buffer, "Speed: %i%%", lv_slider_get_value(slider));

    lv_label_set_text(autonLaunchSpeed, buffer);

    autonSingleFlywheelSpeed = lv_slider_get_value(slider);

    return LV_RES_OK;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
    lv_obj_set_hidden(autonSelectPage, false);
    lv_obj_set_size(autonSelectPage, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style(autonSelectPage, &lv_style_transp_tight);
    lv_obj_set_pos(autonSelectPage, 0, 0);

    autonSelectPageCloseToTextView = createBtn(autonSelectPage, 0, 0, 100, 30, 100, "close");
    lv_obj_set_hidden(autonSelectPageCloseToTextView, true);
    lv_btn_set_action(autonSelectPageCloseToTextView, LV_BTN_ACTION_CLICK, btn_click_action);

    autonRun = createBtn(autonSelectPage, 0, 0, 200, 30, 102, "Run Autonomous");
    lv_obj_align(autonRun, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_obj_set_hidden(autonRun, true);
    lv_btn_set_action(autonRun, LV_BTN_ACTION_CLICK, btn_click_action);

    lv_obj_set_hidden(textViewPage, true);
    lv_obj_set_size(textViewPage, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style(textViewPage, &lv_style_transp_tight);
    lv_obj_set_pos(textViewPage, 0, 0);

    autonPageButton = createBtn(textViewPage, 0, 0, 300, 40, 101, "Open Autonomous Selector");
    lv_obj_set_hidden(autonPageButton, false);
    lv_btn_set_action(autonPageButton, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_obj_set_pos(autonPageButton, LV_HOR_RES - 300, LV_VER_RES - 40);

    lv_label_set_text(autonSelectPageTitle, "Autonomous");
    lv_obj_set_pos(autonSelectPageTitle, 140, 8);

    tabContainer = lv_page_create(autonSelectPage, NULL);
    lv_obj_set_pos(tabContainer, 0, 25);
    lv_page_set_sb_mode(tabContainer, LV_SB_MODE_OFF);
    lv_obj_set_style(tabContainer, &lv_style_transp_tight);
    lv_obj_set_size(tabContainer, LV_HOR_RES, 40);
    lv_page_set_scrl_layout(tabContainer, LV_LAYOUT_ROW_M);
    lv_page_set_scrl_fit(tabContainer, true, false);
    lv_page_set_scrl_height(tabContainer, 40);

    autonSidePage = lv_page_create(autonSelectPage, NULL);
    autonSideNearPage = lv_page_create(autonSidePage, NULL);
    autonSideNearDesc = lv_label_create(autonSideNearPage, NULL);
    autonSideFarPage = lv_page_create(autonSidePage, NULL);
    autonSideFarDesc = lv_label_create(autonSideFarPage, NULL);

    autonNone = createBtn(tabContainer, LV_HOR_RES * 0.05, 0, LV_HOR_RES * 0.225, 40, 0, "None");
    autonRed = createBtn(tabContainer, LV_HOR_RES * 0.275, 0, LV_HOR_RES * 0.225, 40, 1, "Red");
    autonBlue = createBtn(tabContainer, LV_HOR_RES * 0.5, 0, LV_HOR_RES * 0.225, 40, 2, "Blue");
    autonLaunch = createBtn(tabContainer, LV_HOR_RES * 0.725, 0, LV_HOR_RES * 0.225, 40, 3, "Flywheel");

    autonSideNear = createBtn(autonSidePage, LV_HOR_RES * 0.05, 0, LV_HOR_RES * 0.225, 40, 4, "Near Tile");
    autonSideFar = createBtn(autonSidePage, LV_HOR_RES * 0.28, 0, LV_HOR_RES * 0.225, 40, 5, "Far Tile");
    autonSidePlatform = createBtn(autonSidePage, LV_HOR_RES * 0.51, 0, LV_HOR_RES * 0.48, 40, 6, "Platform: low");

    btnSetToggled(autonNone, true);

    btnSetToggled(autonSideNear, true);

    lv_btn_set_action(autonNone, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(autonRed, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(autonBlue, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(autonLaunch, LV_BTN_ACTION_CLICK, btn_click_action);

    lv_btn_set_action(autonSideNear, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(autonSideFar, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_action(autonSidePlatform, LV_BTN_ACTION_CLICK, btn_click_action);

    static lv_style_t defaultButton;

    lv_style_copy(&defaultButton, &lv_style_plain);

    defaultButton.body.radius = 0;

    lv_style_t * greyStyle = createBtnStyle(&defaultButton,
        LV_COLOR_MAKE(150, 150, 150), LV_COLOR_MAKE(200, 200, 200),
        LV_COLOR_MAKE(150, 150, 150), LV_COLOR_MAKE(200, 200, 200),
        LV_COLOR_MAKE(255, 255, 255), LV_COLOR_MAKE(0, 0, 0), autonNone);

    lv_style_t * redStyle = createBtnStyle(&defaultButton,
        LV_COLOR_MAKE(200, 0, 0), LV_COLOR_MAKE(255, 100, 100),
        LV_COLOR_MAKE(200, 0, 0), LV_COLOR_MAKE(255, 100, 100),
        LV_COLOR_MAKE(255, 255, 255), LV_COLOR_MAKE(255, 255, 255), autonRed);

    lv_style_t * blueStyle = createBtnStyle(&defaultButton,
        LV_COLOR_MAKE(0, 0, 200), LV_COLOR_MAKE(100, 100, 255),
        LV_COLOR_MAKE(0, 0, 200), LV_COLOR_MAKE(100, 100, 255),
        LV_COLOR_MAKE(255, 255, 255), LV_COLOR_MAKE(255, 255, 255), autonBlue);

    setBtnStyle(greyStyle, autonLaunch);

    setBtnStyle(redStyle, autonSelectPageCloseToTextView);
    setBtnStyle(greyStyle, autonRun);
    setBtnStyle(greyStyle, autonPageButton);

    setBtnStyle(greyStyle, autonSideNear);
    setBtnStyle(greyStyle, autonSideFar);
    setBtnStyle(greyStyle, autonSidePlatform);

    autonNonePage = lv_page_create(autonSelectPage, NULL);
    autonNoneDesc = lv_label_create(autonNonePage, NULL);
    lv_obj_set_hidden(autonNonePage, false);
    lv_obj_set_pos(autonNonePage, 0, 70);
    lv_obj_set_size(autonNonePage, LV_HOR_RES, LV_VER_RES - 80);
    lv_obj_set_style(autonNonePage, &lv_style_transp_tight);
    lv_label_set_text(autonNoneDesc, "The robot will do nothing.");
    lv_obj_align(autonNoneDesc, NULL, LV_ALIGN_CENTER, 0, 0);

    //side page
    lv_obj_set_hidden(autonSidePage, true);
    lv_obj_set_pos(autonSidePage, 0, 70);
    lv_obj_set_size(autonSidePage, LV_HOR_RES, LV_VER_RES - 80);
    lv_obj_set_style(autonSidePage, &lv_style_transp_tight);

    lv_obj_set_hidden(autonSideNearPage, false);
    lv_obj_set_pos(autonSideNearPage, 0, 50);
    lv_obj_set_size(autonSideNearPage, LV_HOR_RES, LV_VER_RES - 140);
    lv_obj_set_style(autonSideNearPage, &lv_style_transp_tight);
    lv_label_set_text(autonSideNearDesc, "The robot will run autonomous on near tile.\nIt will get a second ball, flip all three flags, \nand drive onto the low platform.");
    lv_obj_align(autonSideNearDesc, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_hidden(autonSideFarPage, true);
    lv_obj_set_pos(autonSideFarPage, 0, 50);
    lv_obj_set_size(autonSideFarPage, LV_HOR_RES, LV_VER_RES - 140);
    lv_obj_set_style(autonSideFarPage, &lv_style_transp_tight);
    lv_label_set_text(autonSideFarDesc, "The robot will run autonomous on far tile.\nIt will get a second ball, flip all two flags in the center, \nand drive onto the low platform.");
    lv_obj_align(autonSideFarDesc, NULL, LV_ALIGN_CENTER, 0, 0);

    autonLaunchPage = lv_page_create(autonSelectPage, NULL);
    autonLaunchSpeed = lv_label_create(autonLaunchPage, NULL);
    autonLaunchSlider = lv_slider_create(autonLaunchPage, NULL);
    autonLaunchDesc = lv_label_create(autonLaunchPage, NULL);
    lv_obj_set_hidden(autonLaunchPage, true);
    lv_obj_set_pos(autonLaunchPage, 0, 70);
    lv_obj_set_size(autonLaunchPage, LV_HOR_RES, LV_VER_RES - 80);
    lv_obj_set_style(autonLaunchPage, &lv_style_transp_tight);

    lv_label_set_text(autonLaunchSpeed, "Speed: 75%");
    lv_obj_align(autonLaunchSpeed, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_size(autonLaunchSlider, LV_HOR_RES - 100, 30);
    lv_obj_align(autonLaunchSlider, NULL, LV_ALIGN_CENTER, 0, 40);
    lv_slider_set_action(autonLaunchSlider, autonLaunchSlider_action);
    lv_bar_set_value(autonLaunchSlider, 75);

    lv_label_set_text(autonLaunchDesc, "The robot will launch one ball at a set speed.");
    lv_obj_align(autonLaunchDesc, NULL, LV_ALIGN_CENTER, 0, 60);


    /*while(true)
    {
        pros::delay(10);
    }*/
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
    lv_obj_set_hidden(autonSelectPage, false);
    lv_obj_set_hidden(textViewPage, true);
    lv_obj_set_hidden(autonSelectPageCloseToTextView, true);
    lv_obj_set_hidden(autonRun, true);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
    lv_obj_set_hidden(autonSelectPage, false);
    lv_obj_set_hidden(textViewPage, true);
    lv_obj_set_hidden(autonSelectPageCloseToTextView, true);
    lv_obj_set_hidden(autonRun, true);
}
