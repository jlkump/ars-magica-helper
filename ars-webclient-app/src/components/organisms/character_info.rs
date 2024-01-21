use crate::components::atoms::{
    age_display::AgeDisplay,
    character_name::CharacterName, 
    character_portrait::CharacterPortrait,
    decrepitude_display::DecrepitudeDisplay,
    flaw_display::FlawDisplay,
    personality_display::PersonalityDisplay,
    soak_size_display::SoakSizeDisplay,
    virtue_display::VirtueDisplay,
    warp_display::WarpDisplay,
    warp_display::WarpIntensity,
    wound_display::WoundDisplay,
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
                        <AgeDisplay true_age={1125} apparent_age={25} class="grid-c-1-2 grid-r-2 widget" />
                        <DecrepitudeDisplay score={1} points={5} class="grid-c-2-3 grid-r-2 widget"/>
                        <WarpDisplay score={1} points={5} warp_intensity={WarpIntensity::None} class="grid-c-3-4 grid-r-2 widget" />
                        <WoundDisplay light={1} medium={0} heavy={0} incapacitating={0} class={"grid-c-1-3 grid-r-3 widget"} />
                        <SoakSizeDisplay soak={1} size={1} encumbrance={1} class={"grid-c-3-4 grid-r-3 widget"} />
                    </div>

                    <div class={"margin-small-top display-grid-2 grid-r-2"}>
                        <VirtueDisplay 
                            virtues={vec![
                                "Virtue 1".to_owned(), 
                                "Virtue 2".to_owned(), 
                                "Virtue 3".to_owned()
                            ]} class="grid-c-1-2 grid-r-1 widget" 
                        />
                        <FlawDisplay 
                            flaws={vec![
                                "Flaw 1".to_owned(), 
                                "Flaw 2".to_owned(), 
                                "Flaw 3".to_owned()
                            ]} class="grid-c-2-3 grid-r-1 widget" 
                        />
                        <PersonalityDisplay 
                            personality={vec![
                                "Personality 1".to_owned(), 
                                "Personality 2".to_owned(), 
                                "Personality 3".to_owned()
                            ]} 
                            scores={vec![
                                1,
                                -3
                            ]}
                            class="grid-c-1-2 grid-r-2 widget" 
                        />
                        // <div class="grid-c-1-2 grid-r-1 widget">
                        //     <h4>{"Virtues"}</h4>
                        // </div>
                        // <div class="grid-c-2-3 grid-r-1 widget">
                        //     <h4>{"Flaws"}</h4>
                        // </div>
                        // <div class="grid-c-1-2 grid-r-2 widget">
                        //     <h4>{"Personality"}</h4>
                        // </div>
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