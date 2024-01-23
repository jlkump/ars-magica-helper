use stylist::yew::styled_component;
use yew::prelude::*;
use yew_icons::{Icon, IconId};

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub flaws: Vec<String>,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(FlawDisplay)]
pub fn flaw_display(props: &Props) -> Html {
    
    html! {
        <div class={&props.class}>        
            <h5 class="underlined">{"Flaws"}</h5>
            <div class="virtue-flaw-container">
                <table>
                    {table_data(&props.flaws)}
                </table>
            </div>
        </div>
    }
}

fn table_data(flaws: &Vec<String>) -> Vec<Html> {
    let mut result = vec![];
    for flaw in flaws {
        result.push(html! {
            <tr>
                <td class="flex">
                    <p class="pad-left-small">{flaw}</p>
                    <Icon icon_id={IconId::HeroiconsMiniSolidLink} class="right size-x-small"/>
                </td>
            </tr>
          });
    }
    result
}