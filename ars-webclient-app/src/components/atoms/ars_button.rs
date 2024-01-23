use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub label: String,
    #[prop_or_default]
    pub onclick: Option<Callback<MouseEvent>>,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(ArsButton)]
pub fn ars_button(props: &Props) -> Html {
    let onclick = {
        let props_onclick = props.onclick.clone();
        Callback::from(move |event: MouseEvent| {
            if let Some(props_onclick) = props_onclick.clone() {
                props_onclick.emit(event);
            }
        })
    };

    html! {
        <button {onclick} class={&props.class}>{&props.label}</button>
    }
}