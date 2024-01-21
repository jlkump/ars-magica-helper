use stylist::yew::styled_component;
use yew::prelude::*;


#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub soak: i32,
    pub size: i32,
    pub encumbrance: i32,
    pub class: Option<String>,
}

#[styled_component(SoakSizeDisplay)]
pub fn soak_size_display(props: &Props) -> Html {
    let soak_display_str = if props.soak > 0 {
        format!("+{}", props.soak)
    } else {
        format!("{}", props.soak)
    };
    let size_display_str = if props.size > 0 {
        format!("+{}", props.size)
    } else {
        format!("{}", props.size)
    };
    let encumbrance_display_str = format!("{}", props.encumbrance);
    html! {
        <table class={&props.class}>
            <tr>
                <td><h5>{"Size: "}</h5></td>
                <td><h5>{size_display_str}</h5></td>
            </tr>
                <tr>
                <td><h5>{"Soak: "}</h5></td>
                <td><h5>{soak_display_str}</h5></td>
            </tr>
            <tr>
                <td><h5>{"Encumbrance: "}</h5></td>
                <td><h5>{encumbrance_display_str}</h5></td>
            </tr>
        </table>
    }
}