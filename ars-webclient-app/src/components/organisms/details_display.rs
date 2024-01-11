use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[styled_component(DetailsDisplay)]
pub fn details_display(_props: &Props) -> Html {
    html! {
        <div class="prevent-select display-grid-1-special grid-r-1 grid-c-2-3 margin-small">
            <div class="grid-r-1 widget">
                <h3>{"View 1"}</h3>
            </div>
            <div class="grid-r-2 widget">
                <h3>{"Select View"}</h3>
            </div>
        </div>
    }
}