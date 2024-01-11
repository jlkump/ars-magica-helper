use stylist::yew::styled_component;
use yew::prelude::*;


#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub names: Vec<String>,
    pub specialties: Vec<String>,
    pub scores: Vec<i32>,
    pub exps: Vec<i32>,
    pub class: Option<String>,
}

#[styled_component(AbilitiesList)]
pub fn abilities_list(props: &Props) -> Html {
    html! {
        <table class={&props.class}>
            <thead>
            <th><h4 class="text-center">{"Name"}</h4></th>
            <th><h5 class="text-center">{"Specialty"}</h5></th>
            <th><h4 class="text-center">{"Score"}</h4></th>
            <th><h4 class="text-center">{"Exp"}</h4></th>
            </thead>
            {table_data(&props.names, &props.specialties, &props.scores, &props.exps)}
        </table>
    }
}

fn table_data(names: &Vec<String>, specialties: &Vec<String>, scores: &Vec<i32>, exps: &Vec<i32>) -> Vec<Html> {
    let mut result = vec![];
    let mut specialties = specialties.iter();
    let mut scores = scores.iter();
    let mut exps = exps.iter();
    for name in names {
        let specialty = specialties.next();
        let score = scores.next();
        let exp: Option<&i32> = exps.next();
        result.push(html! {
            <tr>
                <td><p class="text-center">{name}</p></td>
                if let Some(specialty) = specialty {
                    <td><p class="text-center text-small">{specialty}</p></td>
                } else {
                    <td></td>
                }
                if let Some(score) = score {
                    <td><p class="text-center">{score}</p></td>
                } else {
                    <td><p class="text-center">{0}</p></td>
                }
                if let Some(exp) = exp {
                    <td><p class="text-center">{exp}</p></td>
                } else {
                    <td><p class="text-center">{0}</p></td>
                }
            </tr>
          });
    }
    result
}