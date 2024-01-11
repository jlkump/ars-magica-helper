mod components;

use stylist::yew::{ Global, styled_component };
use yew::prelude::*;

use components::organisms::nav_bar::NavBar;
use components::organisms::character_info::CharacterInfo;

const GLOBAL_STYLESHEET_FILE: &'static str = include_str!("css/global.css");

#[styled_component(App)]
pub fn app() -> Html {
    html! {
        <>
            <Global css={GLOBAL_STYLESHEET_FILE} />
            <NavBar/>
            <div class="page-content">
                <div class="main prevent-select">
                    <h2>{"Character Info"}</h2>
                    <CharacterInfo/>
                </div>
                <div class="detail">
                    <h2>{"Details"}</h2>
                    // <p>{"Some text"}</p>
                </div>
            </div>
        </>
    }
}