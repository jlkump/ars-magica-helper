use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub chars: Vec<String>,
    pub scores: Vec<i32>,
    pub ages: Vec<i32>,
    pub class: Option<String>,
}

#[styled_component(CharacteristicsList)]
pub fn characteristics_list(props: &Props) -> Html {
    
    html! {
        <table class={&props.class}>
            <thead>
            <th><h4 class="text-center">{"Characteristic"}</h4></th>
            <th><h4 class="text-center">{"Score"}</h4></th>
            <th><h4 class="text-center">{"Age"}</h4></th>
            </thead>
            {table_data(&props.chars, &props.scores, &props.ages)}
        </table>
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
                <td><p class="text-center">{characteristic}</p></td>
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