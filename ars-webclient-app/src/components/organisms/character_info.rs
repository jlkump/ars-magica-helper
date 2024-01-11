use crate::components::atoms::{
    age_display::AgeDisplay,
    character_name::CharacterName, 
    character_portrait::CharacterPortrait,
    decrepitude_display::DecrepitudeDisplay,
    warp_display::WarpDisplay,
};

use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[styled_component(CharacterInfo)]
pub fn character_info(_props: &Props) -> Html {
    html! {
        <span>
            <div class={"display-grid-2 margin-small"}>
                <div class={"display-grid-1 grid-r-1 grid-c-1-2"}>
                    <div class={"display-grid-3 grid-r-1"}>
                        <div class={"grid-c-1-2 grid-r-1 widget"}>
                            <CharacterPortrait/>
                        </div>
                        <div class={"grid-c-2-4 grid-r-1 center widget"}>
                            <CharacterName name="Magus Name" />
                        </div>
                        <div class={"grid-c-1-2 grid-r-2 widget"}>
                            <AgeDisplay true_age={25} apparent_age={25}/>
                        </div>
                        <div class={"grid-c-2-3 grid-r-2 widget"}>
                            <DecrepitudeDisplay score={1} points={5}/>
                        </div>
                        <div class={"grid-c-3-4 grid-r-2 widget"}>
                            <WarpDisplay score={1} points={5}/>
                        </div>
                        <div class={"grid-c-1-2 grid-r-3 widget"}>
                            <h3>{"WoundDisplay"}</h3>
                        </div>
                        <div class={"grid-c-2-4 grid-r-3 widget"}>
                            <h3>{"Soak, Size, Encumbrance"}</h3>
                        </div>
                    </div>

                    <div class={"display-grid-2 grid-r-2"}>
                        <div class="grid-c-1-2 grid-r-1 widget">
                            <h3>{"Virtues"}</h3>
                        </div>
                        <div class="grid-c-2-3 grid-r-1 widget">
                            <h3>{"Flaws"}</h3>
                        </div>
                        <div class="grid-c-1-2 grid-r-2 widget">
                            <h3>{"Personality"}</h3>
                        </div>
                        <div class="grid-c-2-3 grid-r-2 widget">
                            <h3>{"Reputation"}</h3>
                        </div>
                    </div>
                </div>

                <div class={"display-grid-1 grid-c-2-3 grid-r-1"}>
                    <div class={"grid-r-1 widget"}>
                        <h3>{"Characteristics"}</h3>
                    </div>
                    <div class={"grid-r-2 widget"}>
                        <h3>{"Abilities"}</h3>
                    </div>
                </div>
            </div>
        </span>
    }
}