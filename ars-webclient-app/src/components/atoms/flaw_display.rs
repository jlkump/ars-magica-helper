use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub flaws: Vec<String>,
    pub class: Option<String>,
}

#[styled_component(FlawDisplay)]
pub fn flaw_display(props: &Props) -> Html {
    
    html! {
        <div class={&props.class}>        
            <h3>{"Flaws"}</h3>
            <table>
                // <thead>
                // <th><h4 class="text-center">{"Virtues"}</h4></th>
                // <th><h4 class="text-center">{"Score"}</h4></th>
                // <th><h4 class="text-center">{"Age"}</h4></th>
                // </thead>
                {table_data(&props.flaws)}
            </table>
        </div>
    }
}

fn table_data(flaws: &Vec<String>) -> Vec<Html> {
    let mut result = vec![];
    for flaw in flaws {
        result.push(html! {
            <tr>
                <td><p class="text-center">{flaw}</p></td>
            </tr>
          });
    }
    result
}