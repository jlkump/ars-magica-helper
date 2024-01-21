use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub score: i32,
    pub points: i32,
    pub class: Option<String>,
}

#[styled_component(DecrepitudeDisplay)]
pub fn decrepitude_display(props: &Props) -> Html {
    let mut class_str = "center ".to_owned();
    if let Some(given) = &props.class {
        class_str = class_str + &given;
    };

    html! {
        <div class={class_str}>
            <h3 class="center">{"Decrepitude"}</h3>
            <table class="center">
                <tr>
                    <td class="no-border"><p>{"Score: "}</p></td>
                    <td class="no-border"><p>{props.score}</p></td>
                </tr>
                <tr>
                    <td class="no-border"><p>{"Points: "}</p></td>
                    <td class="no-border"><p>{props.points}</p></td>
                </tr>
            </table>
        </div>
    }
}