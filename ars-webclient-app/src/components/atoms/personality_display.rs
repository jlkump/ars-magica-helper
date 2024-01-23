use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub personality: Vec<String>,
    #[prop_or(vec![])]
    pub scores: Vec<i32>,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(PersonalityDisplay)]
pub fn personality_display(props: &Props) -> Html {
    
    html! {
        <div class={&props.class}>        
            <h5 class="underlined">{"Personality"}</h5>
            <div class="personality-reputation-container">
                <table>
                    {table_data(&props.personality, &props.scores)}
                </table>
            </div>
        </div>
    }
}

fn table_data(personalities: &Vec<String>, scores: &Vec<i32>) -> Vec<Html> {
    let mut result = vec![];
    let mut scores = scores.iter();
    for personality in personalities {
        let score = scores.next();
        result.push(html! {
            <tr>
                <td><p>{personality}</p></td>
                if let Some(score) = score {
                    if *score > 0 {
                        <td><p class="text-center">{format!("+{}", score)}</p></td>
                    } else {
                        <td><p class="text-center">{score}</p></td>
                    }
                } else {
                    <td><p class="text-center">{0}</p></td>
                }
            </tr>
          });
    }
    result
}