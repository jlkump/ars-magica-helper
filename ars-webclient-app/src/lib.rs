mod components;

use stylist::yew::{ Global, styled_component };
use yew::prelude::*;

use components::atoms::character_portrait::CharacterPortrait;
use components::atoms::character_name::CharacterName;
use components::atoms::decrepitude_display::DecrepitudeDisplay;
use components::atoms::age_display::AgeDisplay;
use components::atoms::warp_display::WarpDisplay;

const GLOBAL_STYLESHEET_FILE: &'static str = include_str!("css/global.css");

#[styled_component(App)]
pub fn app() -> Html {
    
    html! {
        <>
            <Global css={GLOBAL_STYLESHEET_FILE} />
            <h1>{"Ars Magica Helper"}</h1>
            // <CircleBorder radius={50} border={20} class="p"></CircleBorder>
            <div class="center-row">
                <CharacterPortrait></CharacterPortrait>
                <CharacterName name="Magus Name"></CharacterName>
                <AgeDisplay true_age={25} apparent_age={25}></AgeDisplay>
                <DecrepitudeDisplay score={2} points={17}></DecrepitudeDisplay>
                <WarpDisplay score={10} points={10}></WarpDisplay>
            </div>
        </>
    }
}