use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Clone, PartialEq, Eq)]
pub enum WarpIntensity {
    None,
    Weak,
    Strong,
    Intense,
}

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub score: i32,
    pub points: i32,
    #[prop_or_default]
    pub class: Option<String>,
    #[prop_or(WarpIntensity::None)]
    pub warp_intensity: WarpIntensity,
}

#[styled_component(WarpDisplay)]
pub fn warp_display(props: &Props) -> Html {
    let mut class_str = "".to_owned();
    if let Some(given) = &props.class {
        class_str = class_str + &given;
    };

    html! {
        <div class={class_str}>
            <h3 class="center">{"Warping"}</h3>
            <table class="center position-relative warp">
                <tr>
                    <td class="no-border"><p>{"Score: "}</p></td>
                    <td class="no-border"><p>{props.score}</p></td>
                </tr>
                <tr>
                    <td class="no-border"><p>{"Points: "}</p></td>
                    <td class="no-border"><p>{props.points}</p></td>
                </tr>
                if props.warp_intensity == WarpIntensity::Weak 
                    || props.warp_intensity == WarpIntensity::Strong 
                    || props.warp_intensity == WarpIntensity::Intense {
                    <span class="warp-bubble" />
                }
                if props.warp_intensity == WarpIntensity::Strong || props.warp_intensity == WarpIntensity::Intense {
                    <span class="warp-bubble" />
                }
                if props.warp_intensity == WarpIntensity::Intense {
                    <span class="warp-bubble" />
                }
            </table>
        </div>
    }
}