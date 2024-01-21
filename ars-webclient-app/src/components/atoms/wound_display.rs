use stylist::yew::styled_component;
use yew::prelude::*;


#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub light: i32,
    pub medium: i32,
    pub heavy: i32,
    pub incapacitating: i32,
    pub class: Option<String>,
}

#[styled_component(WoundDisplay)]
pub fn wound_display(props: &Props) -> Html {
    html! {
        <div class={&props.class}>
            <h2>{"Wounds"}</h2>
            <table>
                // <thead>
                // <th><h4 class="text-center">{"Name"}</h4></th>
                // <th><h5 class="text-center">{"Specialty"}</h5></th>
                // <th><h4 class="text-center">{"Score"}</h4></th>
                // <th><h4 class="text-center">{"Exp"}</h4></th>
                // </thead>
                <tr>
                <td><h4 class="text-center">{"Light"}</h4></td>
                <td><h5 class="text-center">{props.light}</h5></td>
                </tr>
                <tr>
                <td><h4 class="text-center">{"Medium"}</h4></td>
                <td><h5 class="text-center">{props.medium}</h5></td>
                </tr>
                <tr>
                <td><h4 class="text-center">{"Heavy"}</h4></td>
                <td><h5 class="text-center">{props.heavy}</h5></td>
                </tr>
                <tr>
                <td><h4 class="text-center">{"Incapacitating"}</h4></td>
                <td><h5 class="text-center">{props.incapacitating}</h5></td>
                </tr>
            </table>
        </div>
    }
}