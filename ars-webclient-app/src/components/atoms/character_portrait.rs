use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {}

#[styled_component(CharacterPortrait)]
pub fn character_portrait(_props: &Props) -> Html {
    
    html! {
        <span>
            <div class="center prevent-select">
                <img width="64" height="64" src="https://jlkump.github.io/images/personal-art/Digital-magic-aura.png" />
            </div>
        </span>
    }
}