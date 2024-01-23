use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub chars: Vec<String>,
    pub scores: Vec<i32>,
    pub ages: Vec<i32>,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(CharacteristicsList)]
pub fn characteristics_list(props: &Props) -> Html {
    
    html! {
        <div class="characteristics-container">
            <table class={classes!(&props.class, "color")}>
                <tr>
                    <th><h6 class="text-center">{"Characteristic"}</h6></th>
                    <th><h6 class="text-center">{"Score"}</h6></th>
                    <th><h6 class="text-center">{"Age"}</h6></th>
                </tr>
                {table_data(&props.chars, &props.scores, &props.ages)}
            </table>
        </div>
        // <div>
        // </div>
    }
}

fn table_data(chars: &Vec<String>, scores: &Vec<i32>, ages: &Vec<i32>) -> Vec<Html> {
    let mut result = vec![];
    let mut scores = scores.iter();
    let mut ages = ages.iter();
    for characteristic in chars {
        let score = scores.next();
        let age: Option<&i32> = ages.next();
        result.push(html! {
            <tr>
                <td><p class="pad-left-small">{characteristic}</p></td>
                if let Some(score) = score {
                    <td><p class="text-center">{score}</p></td>
                } else {
                    <td><p class="text-center">{0}</p></td>
                }
                if let Some(age) = age {
                    <td><p class="text-center">{age}</p></td>
                } else {
                    <td><p class="text-center">{0}</p></td>
                }
            </tr>
          });
    }
    result
}