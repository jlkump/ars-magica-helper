use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub score: i32,
    pub points: i32,
}

#[styled_component(WarpDisplay)]
pub fn warp_display(props: &Props) -> Html {
    let score_text = format!("Score: {}", props.score);
    let points_text = format!("Points: {}", props.points);
    html! {
        <span>
            <div class="center prevent-select widget-container">
                <h3>{"Warping"}</h3>
                <p>{score_text}</p>
                <p>{points_text}</p>
                // <div class="warp-circle">
                //     <p class="warp-score-text">{props.score}</p>
                //     <p class="warp-points-text">{props.points}</p>
                // </div>
            </div>
        </span>
    }
}