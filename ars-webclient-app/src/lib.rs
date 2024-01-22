mod components;


use stylist::yew::{ Global, styled_component };
use yew::prelude::*;

use components::organisms::side_bar::SideBar;
use components::organisms::nav_bar::NavBar;
use components::organisms::character_info::CharacterInfo;
use components::organisms::details_display::DetailsDisplay;

const GLOBAL_STYLESHEET_FILE: &'static str = include_str!("css/global.css");

#[styled_component(App)]
pub fn app() -> Html {
    html! {
        <span class="theme-dark">
            <Global css={GLOBAL_STYLESHEET_FILE} />
            <NavBar/>
            <SideBar/>
            <div class="display-grid-2" style="margin-left:20%">
                <CharacterInfo/>
                <DetailsDisplay/>
            </div>
        </span>
    }
}