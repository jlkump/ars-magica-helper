use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {}

#[styled_component(CharacterPortrait)]
pub fn character_portrait(_props: &Props) -> Html {
    
    html! {
        <img class="portrait prevent-select" width="128px" src="https://jlkump.github.io/images/personal-art/Digital-magic-aura.png" />
    }
}