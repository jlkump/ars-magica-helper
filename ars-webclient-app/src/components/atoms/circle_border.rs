use stylist::{yew::styled_component, Style};
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub radius: i32,
    pub border: i32,
    pub class: Option<String>,
}

#[styled_component(CircleBorder)]
pub fn circle_border(props: &Props) -> Html {
    let formated_style = format!(
        r#"
            margin: 5px;
            width: {0}px;
            height: {0}px;
            border: {1}px outset #5C4540;
            -webkit-border-radius: 50%;
            -moz-border-radius: 50%;
            border-radius: 50%;
            /* background-image: radial-gradient(#E8DDCE, #CCB193); */
            background: #CCB193;
        "#, 
        props.radius * 2, 
        props.border
    );

    let style_sheet = Style::new(formated_style).unwrap();

    html! {
        <span>
            <div class={style_sheet}></div>
        </span>
    }
}