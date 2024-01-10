use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub true_age: i32,
    pub apparent_age: i32,
}

#[styled_component(AgeDisplay)]
pub fn age_display(props: &Props) -> Html {
    let apparent_age_text = format!("Apparent: {}", props.apparent_age);
    let true_age_text = format!("True: {}", props.true_age);
    html! {
        <span>
            <div class="prevent-select widget-container">
                <h3>{"Age"}</h3>
                <p>{true_age_text}</p>
                <p>{apparent_age_text}</p>
            </div>
        </span>
    }
}