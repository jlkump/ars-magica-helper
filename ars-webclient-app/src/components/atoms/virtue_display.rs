use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    #[prop_or(vec![])]
    pub virtues: Vec<String>,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(VirtueDisplay)]
pub fn virtue_display(props: &Props) -> Html {
    
    html! {
        <div class={&props.class}>        
            <h3>{"Virtues"}</h3>
            <table>
                // <thead>
                // <th><h4 class="text-center">{"Virtues"}</h4></th>
                // <th><h4 class="text-center">{"Score"}</h4></th>
                // <th><h4 class="text-center">{"Age"}</h4></th>
                // </thead>
                {table_data(&props.virtues)}
            </table>
        </div>
    }
}

fn table_data(virtues: &Vec<String>) -> Vec<Html> {
    let mut result = vec![];
    for virtue in virtues {
        result.push(html! {
            <tr>
                <td><p class="text-center">{virtue}</p></td>
            </tr>
          });
    }
    result
}