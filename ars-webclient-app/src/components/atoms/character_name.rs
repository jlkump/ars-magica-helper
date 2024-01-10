use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub name: String,
}

#[styled_component(CharacterName)]
pub fn character_name(props: &Props) -> Html {
    
    html! {
        <span>
            <div class="center prevent-select widget-container">
                <h2>{props.name.clone()}</h2>
            </div>
        </span>
    }
}