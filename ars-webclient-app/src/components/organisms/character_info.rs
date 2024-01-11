use crate::components::atoms::{
    age_display::AgeDisplay,
    character_name::CharacterName, 
    character_portrait::CharacterPortrait,
    decrepitude_display::DecrepitudeDisplay,
    warp_display::WarpDisplay,
};

use crate::components::molecules::{
    characteristics_list::CharacteristicsList,
    abilities_list::AbilitiesList,
};

use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[styled_component(CharacterInfo)]
pub fn character_info(_props: &Props) -> Html {
    html! {
        <span class="prevent-select grid-r-1 grid-c-1-2">
            // <h2>{"Character Info"}</h2>
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
                            <h4>{"Wounds"}</h4>
                        </div>
                        <div class={"grid-c-2-4 grid-r-3 widget"}>
                            <h4>{"Soak, Size, Encumbrance"}</h4>
                        </div>
                    </div>

                    <div class={"margin-small-top display-grid-2 grid-r-2"}>
                        <div class="grid-c-1-2 grid-r-1 widget">
                            <h4>{"Virtues"}</h4>
                        </div>
                        <div class="grid-c-2-3 grid-r-1 widget">
                            <h4>{"Flaws"}</h4>
                        </div>
                        <div class="grid-c-1-2 grid-r-2 widget">
                            <h4>{"Personality"}</h4>
                        </div>
                        <div class="grid-c-2-3 grid-r-2 widget">
                            <h4>{"Reputation"}</h4>
                        </div>
                    </div>
                </div>

                <div class={"display-grid-1-2 grid-c-2-3 grid-r-1"}>
                    <div class={"grid-r-1 display-grid-1-1 table-container overflow-x-scroll"}>
                        <CharacteristicsList chars={
                            vec!["Intelligence".to_owned(), "Perception".to_owned(), 
                            "Presence".to_owned(), "Communication".to_owned(),
                            "Strength".to_owned(), "Stamina".to_owned(),
                            "Dexterity".to_owned(), "Quickness".to_owned()]} 
                            scores={vec![]} 
                            ages={vec![]} 
                            class={"widget"}
                        />
                    </div>
                    <div class={"grid-r-2 widget no-padding no-border table-container overflow-x-scroll"}>
                        <AbilitiesList 
                            names={vec!["Awareness".to_owned(), 
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned(),
                                "".to_owned()
                            ]} 
                            specialties={vec!["Keeping Watch".to_owned()]} 
                            scores={vec![2]} 
                            exps={vec![21]}
                            class={"widget"}
                        />
                    </div>
                </div>
            </div>
        </span>
    }
}