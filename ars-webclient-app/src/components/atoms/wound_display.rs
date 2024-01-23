use stylist::yew::styled_component;
use yew::prelude::*;
use yew_icons::{Icon, IconId};

use crate::components::atoms::ars_button::ArsButton;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub wound_names: Vec<String>,
    pub wounds: Vec<i32>,
    pub wound_penalties: Vec<i32>,
    #[prop_or_default]
    edit_buttons: bool,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(WoundDisplay)]
pub fn wound_display(props: &Props) -> Html {
    html! {
        <div class={&props.class}>
            <table class="color">
                <tr>
                    <th>
                        <h6 class="">{"Wounds"}</h6>
                    </th>
                    <th>
                        <h6 class="">{"Amount"}</h6>
                    </th>
                    <th class="flex">
                        <h6 class="half-flex vert-center">{"Penalty"}</h6>
                        <Icon icon_id={IconId::FontAwesomeSolidPencil} class="size-x-small"/>
                    </th>
                </tr>
                {table_data(props, props.edit_buttons)}
            </table>
        </div>
    }
}

fn table_data(props: &Props, edit_buttons: bool) -> Vec<Html> {
    let mut wound_text_it = props.wound_names.iter();
    let mut wound_pen_it= props.wound_penalties.iter();
    let mut wounds_it = props.wounds.iter();

    let mut result = vec![];
    let mut sum_penalty = 0;
    let mut sum_count = 0;

    while let Some(wound_text) = wound_text_it.next() {
        let wounds = match wounds_it.next() {
           Some(wounds) => wounds,
           None => &0
        };
        if let Some(wound_pen) = wound_pen_it.next() {
            result.push(html!{
                <tr>
                    <td><p>{wound_text}</p></td>
                    {wound_cell(edit_buttons, wounds)}
                    <td><p class="text-center">{wound_pen}</p></td>
                </tr>
            });
            sum_penalty += wounds * wound_pen;
        } else {
            result.push(html!{
                <tr>
                    <td><p>{wound_text}</p></td>
                    <td><p class="text-center">{wounds}</p></td>
                    <td><p class="text-center">{"N/A"}</p></td>
                </tr>
            });
        }
        sum_count += wounds;
    }
    result.push(html! {
        <tr>
            <td><p class="text-bold">{"Cummulative Total"}</p></td>
            <td><p class="text-center text-bold">{sum_count}</p></td>
            <td><p class="text-center text-bold">{sum_penalty}</p></td>
        </tr> 
    });
    result
}

fn wound_cell(edit_buttons: bool, wounds: &i32) -> Html {
    if edit_buttons {

        html! {
            <td class="flex">
                <ArsButton label="-" class="thirds-flex plus-minus-button" />
                <p class="text-center thirds-flex vert-center">{wounds}</p>
                <ArsButton label="+" class="thirds-flex plus-minus-button" />
            </td>
        }
    } else {
        html! {
            <td>
                <p class="text-center">{wounds}</p>
            </td>
        }
    }
}