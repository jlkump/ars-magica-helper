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
            <div>
                <h2 class="text-colored">{props.name.clone()}</h2>
            </div>
        </span>
    }
}