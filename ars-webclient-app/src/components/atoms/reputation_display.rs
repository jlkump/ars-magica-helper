use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub reputations: Vec<String>,
    pub scores: Vec<i32>,
    pub class: Option<String>,
}

#[styled_component(ReputationDisplay)]
pub fn reputation_display(props: &Props) -> Html {
    
    html! {
        <div class={&props.class}>        
            <h3>{"Reputation"}</h3>
            <table>
                {table_data(&props.reputations, &props.scores)}
            </table>
        </div>
    }
}

fn table_data(reputations: &Vec<String>, scores: &Vec<i32>) -> Vec<Html> {
    let mut result = vec![];
    let mut scores = scores.iter();
    for reputation in reputations {
        let score = scores.next();
        result.push(html! {
            <tr>
                <td><p class="text-center">{reputation}</p></td>
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