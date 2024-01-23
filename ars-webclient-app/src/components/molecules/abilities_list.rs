use stylist::yew::styled_component;
use yew::prelude::*;


#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub names: Vec<String>,
    pub specialties: Vec<String>,
    pub scores: Vec<i32>,
    pub exps: Vec<i32>,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(AbilitiesList)]
pub fn abilities_list(props: &Props) -> Html {
    html! {
        <div class="abilities-container">
            <table class={classes!(&props.class, "color")}>
                <tr>
                    <th colspan="4"><h5>{"Abilities"}</h5></th>
                </tr>
                <tr>
                    <td><h6 class="underlined text-center">{"Name"}</h6></td>
                    <td><h6 class="underlined text-center">{"Specialty"}</h6></td>
                    <td><h6 class="underlined text-center">{"Score"}</h6></td>
                    <td><h6 class="underlined text-center">{"Exp"}</h6></td>
                </tr>
                {table_data(&props.names, &props.specialties, &props.scores, &props.exps)}
            </table>
        </div>
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