use stylist::yew::styled_component;
use yew::prelude::*;
use yew_icons::{Icon, IconId};

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
            <h5 class="underlined">{"Virtues"}</h5>
            <div class="virtue-flaw-container">        
                <table>
                    {table_data(&props.virtues)}
                </table>
            </div>
        </div>
    }
}

fn table_data(virtues: &Vec<String>) -> Vec<Html> {
    let mut result = vec![];
    for virtue in virtues {
        result.push(html! {
            <tr>
                <td class="flex">
                    <p class="pad-left-small">{virtue}</p>
                    <Icon icon_id={IconId::HeroiconsMiniSolidLink} class="right size-x-small"/>
                    // <Icon icon_id={IconId::LucideMoreHorizontal} class="right size-x-small"/>
                </td>
            </tr>
          });
    }
    result
}