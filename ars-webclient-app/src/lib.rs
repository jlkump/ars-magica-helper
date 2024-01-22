mod components;

use components::organisms::page_display::PageDisplay;

use stylist::yew::{ Global, styled_component };
use yew::prelude::*;

const GLOBAL_STYLESHEET_FILE: &'static str = include_str!("css/global.css");

#[styled_component(App)]
pub fn app() -> Html {
    html! {
        <span class="theme-dark">
            <Global css={GLOBAL_STYLESHEET_FILE} />
            <PageDisplay />
        </span>
    }
}