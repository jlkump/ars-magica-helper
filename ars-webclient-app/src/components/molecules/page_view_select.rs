use stylist::yew::styled_component;
use yew::prelude::*;
use yew_icons::{Icon, IconId};

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    #[prop_or(false)]
    magus_options: bool,
}

#[styled_component(PageViewSelect)]
pub fn page_view_select(props: &Props) -> Html {
    
    let selected_index = 0;

    html! {
        <div class="view-select">        
            <ul>
                {button_data(selected_index, props.magus_options)}
            </ul>
        </div>
    }
}

fn button_data(selected_index: usize, show_magus_options: bool) -> Vec<Html> {
    let mut result = vec![];
    let icon_list = vec![IconId::FontAwesomeSolidTimeline, IconId::FontAwesomeSolidBook, IconId::LucideBoxes,
            IconId::FontAwesomeSolidPlusMinus, IconId::FontAwesomeSolidBolt, IconId::FontAwesomeSolidFlaskVial, IconId::FontAwesomeSolidFeather, IconId::HeroiconsSolidMap, IconId::BootstrapJournalText];

    for (ind, icon) in icon_list.iter().enumerate() {
        result.push(html! {
            if ind == selected_index {
                <li class="selected">
                    <Icon icon_id={*icon} />
                </li>
            } else {
                <li>
                    <Icon icon_id={*icon} />
                </li>  
            }
        });
    }

    result
}