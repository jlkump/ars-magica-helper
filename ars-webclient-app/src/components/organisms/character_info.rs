use crate::components::atoms::{
    age_display::AgeDisplay,
    character_name::CharacterName, 
    character_portrait::CharacterPortrait,
    decrepitude_display::DecrepitudeDisplay,
    flaw_display::FlawDisplay,
    personality_display::PersonalityDisplay,
    reputation_display::ReputationDisplay,
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
    // Character display info struct?
    // Made of Character Portrait info,
    // character name info, Age Display Info,
    // Virtue Display Info, etc...
}

#[styled_component(CharacterInfo)]
pub fn character_info(_props: &Props) -> Html {
    html! {
        <span class="prevent-select grid-r-1 grid-c-1-2 content">
            // <h2>{"Character Info"}</h2>
            <div class={"display-grid-2 margin-small"}>
                <div class={"display-grid-1 grid-r-1 grid-c-1-2"}>
                    <div class={"block"}>
                        <div class={"flex"}>
                            <CharacterPortrait/>
                            <CharacterName name="Magus Name" />
                        </div>
                        <hr/>
                        <div class={"flex"}>
                            <AgeDisplay true_age={25} apparent_age={25} class="half-flex" />
                            <SoakSizeDisplay soak={1} size={1} encumbrance={1} class="half-flex"/>
                        </div>
                        <hr/>
                        <div class="flex">
                            <DecrepitudeDisplay score={1} points={5} class="half-flex"/>
                            <WarpDisplay score={1} points={5} class="half-flex"/>
                        </div>
                        <br/>
                        <WoundDisplay 
                            wound_names={vec!["Light".to_owned(), "Medium".to_owned(), "Heavy".to_owned(), "Incapacitating".to_owned()]} 
                            wounds={vec![0, 1, 1, 0]}
                            wound_penalties={vec![-1, -3, -5]}
                        />
                        <br/>
                        <div class="flex">
                            <VirtueDisplay 
                                virtues={vec![
                                    "Virtue 1".to_owned(), 
                                    "Virtue 2".to_owned(), 
                                    "Virtue 3".to_owned()
                                ]} class="half-flex" 
                            />
                            <FlawDisplay 
                                flaws={vec![
                                    "Flaw 1".to_owned(), 
                                    "Flaw 2".to_owned(), 
                                    "Flaw 3".to_owned()
                                ]} class="half-flex" 
                            />
                        </div>
                        <hr/>
                        <div class="flex">
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
                                class="half-flex" 
                            />
                            <ReputationDisplay
                                reputations={vec![
                                    "Reputation 1".to_owned(), 
                                    "Reputation 2".to_owned(), 
                                    "Reputation 3".to_owned()
                                ]} 
                                scores={vec![
                                    2,
                                    -1
                                ]}
                                class="half-flex"    
                            />
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