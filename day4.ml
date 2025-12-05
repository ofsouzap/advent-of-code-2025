open! Core

[@@@warning "-32"]

let max_neighbours_excl = 4

let list_cross xs ys =
  let open List.Cartesian_product.Let_syntax in
  let%bind x = xs in
  let%bind y = ys in
  return (x, y)

module Grid : sig
  type t

  val all_poss : t -> (int * int) list
  val height : t -> int
  val width : t -> int
  val parse : string -> t
  val print : ?mark_poss:(int * int) list -> t -> unit
  val get : t -> int * int -> bool
  val clear_poss : t -> (int * int) list -> t
end = struct
  type t = Grid of bool array array

  let all_poss (Grid grid) =
    let h = Array.length grid in
    let w = Array.length grid.(0) in
    list_cross (List.init h ~f:Fn.id) (List.init w ~f:Fn.id)

  let height (Grid grid) = Array.length grid
  let width (Grid grid) = Array.length grid.(0)

  let parse input =
    let lines = String.split ~on:'\n' input in
    let grid =
      Array.of_list
        (List.map lines ~f:(fun line ->
             Array.of_list
               (List.map (String.to_list line) ~f:(fun c -> equal_char c '@'))))
    in
    Grid grid

  let print ?(mark_poss = []) (Grid grid) =
    Array.iteri grid ~f:(fun i row ->
        Array.iteri row ~f:(fun j cell ->
            Printf.printf "%c"
              (if
                 List.mem mark_poss (i, j) ~equal:(fun (x1, y1) (x2, y2) ->
                     x1 = x2 && y1 = y2)
               then 'x'
               else if cell then '@'
               else '.'));
        Printf.printf "\n")

  let get (Grid grid as t) (i, j) =
    if i < 0 || j < 0 || i >= height t || j >= width t then false
    else grid.(i).(j)

  let clear_poss (Grid grid) poss =
    let new_grid = Array.map grid ~f:Array.copy in
    List.iter poss ~f:(fun (i, j) -> new_grid.(i).(j) <- false);
    Grid new_grid
end

let can_use_pos ~grid (i, j) : bool =
  let offsets =
    list_cross [ -1; 0; 1 ] [ -1; 0; 1 ]
    |> List.filter ~f:(fun (dx, dy) -> not (dx = 0 && dy = 0))
  in
  let rec aux acc_count = function
    | [] ->
        assert (acc_count < max_neighbours_excl);
        true
    | (di, dj) :: rem ->
        let pos = (i + di, j + dj) in
        if Grid.get grid pos then
          let new_count = acc_count + 1 in
          if new_count >= max_neighbours_excl then false else aux new_count rem
        else aux acc_count rem
  in
  aux 0 offsets

let solve_grid_part1 grid =
  let valid_poss =
    List.filter (Grid.all_poss grid) ~f:(fun pos ->
        Grid.get grid pos && can_use_pos ~grid pos)
  in
  (valid_poss, List.length valid_poss)

let solve_grid_part2 =
  let rec aux acc =
    let valid_poss =
      List.filter (Grid.all_poss acc) ~f:(fun pos ->
          Grid.get acc pos && can_use_pos ~grid:acc pos)
    in
    match valid_poss with
    | [] -> 0
    | _ :: _ -> List.length valid_poss + aux (Grid.clear_poss acc valid_poss)
  in
  fun grid -> aux grid

let () =
  let grid = Grid.parse (In_channel.read_all (Sys.get_argv ()).(1)) in
  (* let _valid_poss, res = solve_grid_part1 grid in *)
  let res = solve_grid_part2 grid in
  printf "Solution: %d\n" res
