use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub true_age: i32,
    pub apparent_age: i32,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(AgeDisplay)]
pub fn age_display(props: &Props) -> Html {
    // Class string here incase we have props that don't need to be exposed
    let mut class_str = "center ".to_owned();
    if let Some(given) = &props.class {
        class_str = class_str + &given;
    };

    html! {
        <div class={class_str}>
            // <h3 class="center">{"Age"}</h3>
            <table class="center">
                <tr>
                    <td class="no-border"><p>{"True Age: "}</p></td>
                    <td class="no-border pad-left-small"><p class="text-center">{props.true_age}</p></td>
                </tr>
                <tr>
                    <td class="no-border"><p>{"Apparent Age: "}</p></td>
                    <td class="no-border pad-left-small"><p class="text-center">{props.apparent_age}</p></td>
                </tr>
            </table>
        </div>
    }
}