mod components;

use stylist::yew::{ Global, styled_component };
use yew::prelude::*;

use components::organisms::nav_bar::NavBar;
use components::organisms::character_info::CharacterInfo;
use components::organisms::details_display::DetailsDisplay;

const GLOBAL_STYLESHEET_FILE: &'static str = include_str!("css/global.css");

#[styled_component(App)]
pub fn app() -> Html {
    html! {
        <>
            <Global css={GLOBAL_STYLESHEET_FILE} />
            <NavBar/>
            <div class="display-grid-2">
                <CharacterInfo/>
                <DetailsDisplay/>
            </div>
        </>
    }
}