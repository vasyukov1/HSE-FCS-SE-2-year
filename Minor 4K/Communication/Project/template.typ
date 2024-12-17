#let template(cfg: none, body) = {
    assert(cfg != none)
    assert(cfg.project != none)
    assert(cfg.project.code != none)
    assert(cfg.project.name != none)
    assert(cfg.students != none)
    assert(cfg.university_name != none)
    assert(cfg.faculty_name != none)
    assert(cfg.edu_program_name != none)
    assert(cfg.year != none)

    let un(n) = "_" * n

    let approval_page_code = cfg.project.code + "-ЛУ"

    let approval_page = {
        let top_banner = [
            #set par(spacing: 0.65em)

            #text(weight: "bold", cfg.university_name)

            #cfg.faculty_name

            #cfg.edu_program_name
        ]

        let center_banner = [
            #set text(size: 14pt, weight: "bold")
            #set par(spacing: 2em)

            #par(spacing: 0.65em, cfg.project.name)

            Проект
        ]

        let student_info = align(right)[
            #set par(spacing: 1em)

            Исполнители:

            Группа 4

            #cfg.students.map(
              student => [
                Студент группы #student.group

                #student.name / 
                
                #student.email
              ]
            ).join[#linebreak() #linebreak()]
        ]

        let bottom_banner = [
            #set text(weight: "bold")

            #cfg.year
        ]

        page(
            header: none,
            footer: none,
            margin: (
                left: 20mm,
                right: 10mm,
                top: 25mm,
                bottom: 15mm,
            ),
        )[
            #set align(center)

            #grid(
                columns: (1fr),
                row-gutter: 1fr,
                top_banner,
                center_banner,
                student_info,
                bottom_banner,
            )
        ]

        counter(page).update(1)
    }

    let outline = {
        pagebreak(weak: true)

        {
            set align(center)
            set text(weight: "bold")

            [СОДЕРЖАНИЕ]
        }

        outline(
            title: none,
            indent: 5mm,
        )
    }

    let outline_and_normal_pages = {
        set page(
            margin: (
                top: 25mm,
                left: 20mm,
                right: 10mm,
                bottom: 47mm,
            ),
            header: [
                #set align(center)
                #set text(weight: "bold")

                #context counter(page).display()
            ],
        )

        set par(
            justify: true,
            leading: 1em,
        )

        set heading(numbering: "1.")

        show heading.where(level: 1): h => {
            set align(center)
            set text(weight: "bold", size: 12pt)

            pagebreak(weak: true)
            if h.numbering != none [
                #counter(heading).display(h.numbering) #h.body
            ] else [
                #h.body
            ]

        }

        show heading.where(level: 2): h => {
            set text(weight: "bold", size: 12pt)

            [#counter(heading).display() #h.body]
        }

        pagebreak(weak: true)

        outline
        body
    }

    set text(
        lang: "ru",
        size: 12pt,
        font: "Times New Roman"
    )

    approval_page
    outline_and_normal_pages
}
